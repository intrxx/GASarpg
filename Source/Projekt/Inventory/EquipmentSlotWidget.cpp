// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"

#include "AbilitySystemComponent.h"
#include "ItemObject.h"
#include "ItemSlotWidget.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "MainCharacterInventory.h"
#include "WeaponItemObject.h"
#include "Projekt/MainCharacter/MainCharacterPlayerController.h"
#include "Components/Image.h"
#include "Projekt/GAS/GASGameplayAbility.h"

void UEquipmentSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());
	MainCharacter = Cast<AMainCharacter>(PC->GetCharacter());

	Background->SetBrushFromTexture(BackgroundTexture);
	Thumbnail->SetVisibility(ESlateVisibility::Collapsed); 
}


void UEquipmentSlotWidget::ButtonClicked()
{
	// Check if we hold any item in PC
	if(PC->ItemSlotWidget)
	{
		// Swap around PC item with equipped item
		if(PC->ItemSlotWidget->ItemObject->ItemType == SlotType && ItemObject)
		{
			SwapItemsAround();
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Changed 2 items?"));
		}
		// If slot is empty put item in
		else if(PC->ItemSlotWidget->ItemObject->ItemType == SlotType)
		{
			AddItemToSlot();
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Added item to slot?"));

			ChangeAbility(ItemObject);
		}
	}
	// Remove item from slot if not holding anything
	else if(ItemObject)
	{
		RestoreDefaultAbility(ItemObject);
		
		RemoveItemFromSlot();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Removed item from slot"));
	}
	
	MainCharacter->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
}

void UEquipmentSlotWidget::OnHover()
{
	if(PC->ItemSlotWidget)
	{
		if(PC->ItemSlotWidget->ItemObject->ItemType == SlotType)
		{
			Background->SetColorAndOpacity(FLinearColor(0.0f, 0.3f, 0.0f));
			Thumbnail->SetColorAndOpacity(FLinearColor(0.0f, 0.3f, 0.0f));
		}
		else
		{
			Background->SetColorAndOpacity(FLinearColor(0.3f, 0.0f, 0.0f));
			Thumbnail->SetColorAndOpacity(FLinearColor(0.3f, 0.0f, 0.0f));
		}
	}
	else
	{
		Background->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
		Thumbnail->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
	}
}

void UEquipmentSlotWidget::OnUnhover()
{
	Background->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
	Thumbnail->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
}


void UEquipmentSlotWidget::RemoveItemFromSlot()
{
	PC->CreateItemSlotWidget(ItemObject);

	MainCharacter->GetInventoryComponent()->EquipmentArray.RemoveAt(MainCharacter->GetInventoryComponent()->InventoryArray.Find(ItemObject));

	Thumbnail->SetVisibility(ESlateVisibility::Collapsed);
	ItemObject = nullptr;
		
	OnHover();
}

void UEquipmentSlotWidget::AddItemToSlot()
{
	ItemObject = PC->ItemSlotWidget->ItemObject;
	ItemObject->InventoryIndex = SlotIndex;
	Thumbnail->SetBrushFromTexture(ItemObject->Thumbnail);
	Thumbnail->SetVisibility(ESlateVisibility::Visible);

	MainCharacter->GetInventoryComponent()->EquipmentArray.Add(ItemObject);

	PC->RemoveItemSlotWidget();

	OnUnhover();
}

void UEquipmentSlotWidget::SwapItemsAround()
{
	UItemObject* TempObject = DuplicateObject(ItemObject, nullptr);

	MainCharacter->GetInventoryComponent()->EquipmentArray.RemoveAt(MainCharacter->GetInventoryComponent()->InventoryArray.Find(ItemObject));
			
	ItemObject = PC->ItemSlotWidget->ItemObject;
	ItemObject->InventoryIndex = SlotIndex;
	Thumbnail->SetBrushFromTexture(ItemObject->Thumbnail);
	Thumbnail->SetVisibility(ESlateVisibility::Visible);

	MainCharacter->GetInventoryComponent()->EquipmentArray.Add(ItemObject);
				
	PC->RemoveItemSlotWidget();
	PC->CreateItemSlotWidget(TempObject);

	OnHover();
}

  void UEquipmentSlotWidget::ChangeAbility(UItemObject* Item)
{
	UWeaponItemObject* Trinket = Cast<UWeaponItemObject>(Item);

	if(Trinket)
	{
		TArray<FGameplayAbilitySpec> Abilities2 = MainCharacter->GetAbilitySystemComponent()->GetActivatableAbilities();
		UGASGameplayAbility* Ability = Cast<UGASGameplayAbility>(Abilities2[Trinket->InputIndex].Ability);

		Trinket->Equip(Ability);
		
	}
}

void UEquipmentSlotWidget::RestoreDefaultAbility(UItemObject* Item)
{
	UWeaponItemObject* Trinket = Cast<UWeaponItemObject>(Item);

	if(Trinket)
	{
		TArray<FGameplayAbilitySpec> Abilities2 = MainCharacter->GetAbilitySystemComponent()->GetActivatableAbilities();
		UGASGameplayAbility* Ability = Cast<UGASGameplayAbility>(Abilities2[Trinket->InputIndex].Ability);

		Trinket->Unequip(Ability);
	}
}
