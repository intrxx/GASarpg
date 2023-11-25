// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "AIController.h"
#include "Projekt/Inventory/ChestBase.h"
#include "Projekt/Inventory/ChestComponent.h"
#include "Projekt/AI/EnemyBase.h"
#include "EngineUtils.h"
#include "Projekt/Inventory/ItemObject.h"
#include "Projekt/Saving/LoadingWidget.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Projekt/Inventory/MainCharacterInventory.h"
#include "Projekt/Inventory/Pickup.h"
#include "Projekt/Saving/SaveClass.h"
#include "Projekt/Saving/SaveSlotWidget.h"
#include "Projekt/Saving/SavingWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"


void UMyGameInstance::Init()
{
	Super::Init();
}

UMyGameInstance::UMyGameInstance()
{
	
}

void UMyGameInstance::SaveGame(USaveSlotWidget* SaveSlotWidget)
{
	if (USaveClass* SaveGameInstance = Cast<USaveClass>(UGameplayStatics::CreateSaveGameObject(USaveClass::StaticClass())))
	{
		// Sets selected slot name for thumbnail
		SaveGameInstance->SaveSlotName = SaveSlotWidget->SaveSlotName;
		
		// Set data on the savegame object.
		SavePlayerData(SaveGameInstance);
		SaveChestData(SaveGameInstance);
		SavePickupData(SaveGameInstance);
		SaveEnemyData(SaveGameInstance);
		
		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotWidget->SaveSlotName, 0))
		{
			UTexture2D* Snapshot = UKismetRenderingLibrary::ImportFileAsTexture2D(GetWorld(), SaveGameInstance->ScreenshotPath);
			if(Snapshot)
			{
				USavingWidget* SavingWidget = Cast<USavingWidget>(SaveSlotWidget->OwningWidget);
				if(SavingWidget)
				{
					SavingWidget->ImageSnapshot->SetBrushFromTexture(Snapshot);
					SavingWidget->RemoveFromParent();
				}
			}
		}
		// fixes saving on a new slot
		USavingWidget* SavingWidget = Cast<USavingWidget>(SaveSlotWidget->OwningWidget);
		if(SavingWidget)
		{
			SavingWidget->RemoveFromParent();
		}
	}
}

void UMyGameInstance::LoadGame(USaveSlotWidget* SaveSlotWidget)
{
	// Retrieve and cast the USaveGame object to UMySaveGame.
	if (USaveClass* LoadedGame = Cast<USaveClass>(UGameplayStatics::LoadGameFromSlot(SaveSlotWidget->SaveSlotName, 0)))
	{
		LoadPlayerData(LoadedGame);
		LoadChestData(LoadedGame);
		LoadPickupData(LoadedGame);
		LoadEnemyData(LoadedGame);
		

		// Fixes camera not changing location on load
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetTimerManager().SetTimer(LoadPauseTimerHandle, this, &UMyGameInstance::LoadPauseHandle, 0.01f, false);
		
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
	}
	else
	{
		GetEngine()->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Failed load"));
	}
}

void UMyGameInstance::LoadSaveInfo(USaveSlotWidget* SaveSlotWidget, UUserWidget* InfoWidget)
{
	if (USaveClass* LoadedGame = Cast<USaveClass>(UGameplayStatics::LoadGameFromSlot(SaveSlotWidget->SaveSlotName, 0)))
	{
		if(USavingWidget* SavingWidget = Cast<USavingWidget>(InfoWidget))
		{
			SavingWidget->TextDate->SetVisibility(ESlateVisibility::Visible);
			SavingWidget->TextDate->SetText(FText::FromString(LoadedGame->CreationTime.ToString()));
			SavingWidget->DeleteButton->SetVisibility(ESlateVisibility::Visible);

			UTexture2D* Snapshot = UKismetRenderingLibrary::ImportFileAsTexture2D(GetWorld(), LoadedGame->ScreenshotPath);
			if(Snapshot)
			{
				SavingWidget->ImageSnapshot->SetBrushFromTexture(Snapshot);
			}
		}
		else if(ULoadingWidget* LoadingWidget = Cast<ULoadingWidget>(InfoWidget))
		{
			LoadingWidget->TextDate->SetVisibility(ESlateVisibility::Visible);
			LoadingWidget->TextDate->SetText(FText::FromString(LoadedGame->CreationTime.ToString()));
			LoadingWidget->DeleteButton->SetVisibility(ESlateVisibility::Visible);

			UTexture2D* Snapshot = UKismetRenderingLibrary::ImportFileAsTexture2D(GetWorld(), LoadedGame->ScreenshotPath);
			if(Snapshot)
			{
				LoadingWidget->ImageSnapshot->SetBrushFromTexture(Snapshot);
			}
		}
	}
	// No save found
	else
	{
		if(USavingWidget* SavingWidget = Cast<USavingWidget>(InfoWidget))
		{
			SavingWidget->TextDate->SetVisibility(ESlateVisibility::Collapsed);
			SavingWidget->DeleteButton->SetVisibility(ESlateVisibility::Collapsed);
			SavingWidget->ImageSnapshot->SetBrushFromTexture(SaveSlotWidget->DefaultThumbnail);
			
		}
		else if(ULoadingWidget* LoadingWidget = Cast<ULoadingWidget>(InfoWidget))
		{
			LoadingWidget->TextDate->SetVisibility(ESlateVisibility::Collapsed);
			LoadingWidget->DeleteButton->SetVisibility(ESlateVisibility::Collapsed);
			LoadingWidget->ImageSnapshot->SetBrushFromTexture(SaveSlotWidget->DefaultThumbnail);
		}
	}
}

void UMyGameInstance::SavePlayerData(USaveClass* Save)
{
	Save->PlayerData.PlayerTransform = MainCharacter->GetTransform();

	// Saves date of creating save
	Save->CreationTime = FDateTime::Now();
	
	FScreenshotRequest* ScreenshotRequest = new FScreenshotRequest();
	ScreenshotRequest->RequestScreenshot(Save->SaveSlotName, false, false);
	
	Save->ScreenshotPath = ScreenshotRequest->GetFilename();
	

	for (auto Item : MainCharacter->GetInventoryComponent()->InventoryArray)
	{
		Save->PlayerData.InventoryIndex.Add(Item->InventoryIndex);
		Save->PlayerData.AmountIndex.Add(Item->Amount);
		Save->PlayerData.InventoryClassArray.Add(Item->GetClass());
	}

	for (auto Item : MainCharacter->GetInventoryComponent()->EquipmentArray)
	{
		Save->PlayerData.EquipmentIndex.Add(Item->SlotIndex);
		Save->PlayerData.EquipmentClassArray.Add(Item->GetClass());
	}
}

void UMyGameInstance::LoadPlayerData(USaveClass* Save)
{
	MainCharacter->SetActorTransform(Save->PlayerData.PlayerTransform);

	MainCharacter->GetInventoryComponent()->InventoryArray.Empty();
	MainCharacter->GetInventoryComponent()->EquipmentArray.Empty();

	for(int i = 0; i < Save->PlayerData.InventoryClassArray.Num(); i++)
	{
		UItemObject* ItemObject = NewObject<UItemObject>(this, Save->PlayerData.InventoryClassArray[i],  NAME_None, RF_NoFlags,
			Save->PlayerData.InventoryClassArray[i]->GetDefaultObject(), true);

		MainCharacter->GetInventoryComponent()->InventoryArray.Add(ItemObject);
		
		MainCharacter->GetInventoryComponent()->InventoryArray[i]->InventoryIndex = Save->PlayerData.InventoryIndex[i];
		MainCharacter->GetInventoryComponent()->InventoryArray[i]->Amount = Save->PlayerData.AmountIndex[i];
	}

	for(int i = 0; i < Save->PlayerData.EquipmentClassArray.Num(); i++)
	{
		UItemObject* ItemObject = NewObject<UItemObject>(this, Save->PlayerData.EquipmentClassArray[i],  NAME_None, RF_NoFlags,
			Save->PlayerData.EquipmentClassArray[i]->GetDefaultObject(), true);
		
		MainCharacter->GetInventoryComponent()->EquipmentArray.Add(ItemObject);
		
		MainCharacter->GetInventoryComponent()->EquipmentArray[i]->SlotIndex = Save->PlayerData.EquipmentIndex[i];
	}
}

void UMyGameInstance::SaveChestData(USaveClass* Save)
{
	
	for (auto ChestActor : ChestArray)
	{
		AChestBase* Chest = Cast<AChestBase>(ChestActor);

		for (auto Item : Chest->GetChestComponent()->InventoryArray)
		{
			Chest->ChestData.InventoryArray.Add(Item);
			Chest->ChestData.InventoryIndex.Add(Item->InventoryIndex);
		}
		Save->ChestArray.Add(Chest->ChestData);
	}
	
}

void UMyGameInstance::LoadChestData(USaveClass* Save)
{
	
	for(int i = 0; i < ChestArray.Num(); i++)
	{
		AChestBase* Chest = Cast<AChestBase>(ChestArray[i]);

		Chest->GetChestComponent()->InventoryArray = Save->ChestArray[i].InventoryArray;
		
		for (int j = 0; j < Chest->GetChestComponent()->InventoryArray.Num(); j++)
		{
			Chest->GetChestComponent()->InventoryArray[j]->InventoryIndex = Save->ChestArray[i].InventoryIndex[j];
		}
	}
}

void UMyGameInstance::SavePickupData(USaveClass* Save)
{
	Save->PickupArray.Empty();
	Save->PickupTransformArray.Empty();
	
	for (auto PickupActor : PickupArray)
	{
		Save->PickupArray.Add(PickupActor->GetClass());
		Save->PickupTransformArray.Add(PickupActor->GetActorTransform());
	}
}

void UMyGameInstance::LoadPickupData(USaveClass* Save)
{
	for (auto Pickup : PickupArray)
	{
		Pickup->Destroy();
	}

	PickupArray.Empty();
	
	int PickupIndex = 0;
	
	for (auto PickupSave : Save->PickupArray)
	{
		Save->PickupTransformArray[PickupIndex].RemoveScaling();
		
		PickupArray.Add(GetWorld()->SpawnActor<APickup>(PickupSave, Save->PickupTransformArray[PickupIndex]));

		PickupIndex++;
	}
}

void UMyGameInstance::SaveEnemyData(USaveClass* Save)
{
	Save->EnemyArray.Empty();
	
	for (auto Enemy : EnemyArray)
	{
		FEnemyData EnemyData;

		EnemyData.Transform = Enemy->GetTransform();
		EnemyData.EnemyClass = Enemy->GetClass();
		
		Save->EnemyArray.Add(EnemyData);
	}
}

void UMyGameInstance::LoadEnemyData(USaveClass* Save)
{
	for (auto Enemy : EnemyArray)
	{
		Cast<AEnemyBase>(Enemy)->Die();
	}

	EnemyArray.Empty();

	int EnemyIndex = 0;

	for(auto EnemySave : Save->EnemyArray)
	{
		Save->EnemyArray[EnemyIndex].EnemyClass;

		AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemySave.EnemyClass, Save->EnemyArray[EnemyIndex].Transform);

		SpawnedEnemy->SpawnDefaultController();

		EnemyArray.Add(SpawnedEnemy);

		EnemyIndex++;
	}
}

void UMyGameInstance::LoadPauseHandle()
{
	GetWorld()->GetFirstPlayerController()->SetPause(true);
}

