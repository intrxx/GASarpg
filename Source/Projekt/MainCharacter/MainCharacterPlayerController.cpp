// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterPlayerController.h"

#include "Projekt/Inventory/ItemObject.h"
#include "MainCharacterHUDWidget.h"
#include "Projekt/Inventory/ItemSlotWidget.h"
#include "MainPlayerState.h"
#include "Projekt/Inventory/WidgetInventory.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Projekt/AI/EnemyBase.h"
#include "Projekt/UI/PFloatingDamageNumberWComponent.h"

AMainCharacterPlayerController::AMainCharacterPlayerController()
{
	bEnableMouseOverEvents = true;
}

void AMainCharacterPlayerController::CreateItemSlotWidget(UItemObject* ItemToCopy)
{
	ItemSlotWidget = Cast<UItemSlotWidget>(CreateWidget(this, ItemSlotWidgetClass));

	if(ItemSlotWidget)
	{
		ItemSlotWidget->ItemObject = ItemToCopy;
		ItemSlotWidget->Thumbnail->SetBrushFromTexture(ItemToCopy->Thumbnail);
		ItemSlotWidget->Thumbnail->SetVisibility(ESlateVisibility::Visible);

		if(ItemToCopy->Amount > 1)
		{
			ItemSlotWidget->AmountText->SetText(FText::AsNumber(ItemToCopy->Amount));
			ItemSlotWidget->AmountText->SetVisibility(ESlateVisibility::Visible);
		}
		
		ItemSlotWidget->AddToViewport();

		ItemSlotWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void AMainCharacterPlayerController::RemoveItemSlotWidget()
{
	ItemSlotWidget->RemoveFromViewport();
	ItemSlotWidget = nullptr;
}

void AMainCharacterPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(ItemSlotWidget)
	{
		FVector2D CursorPos;

		GetMousePosition(CursorPos.X, CursorPos.Y);

		// avoids clicking on preview widget
		CursorPos.X += 1;
	
		ItemSlotWidget->SetPositionInViewport(CursorPos);
	}

	if(ChestWidget && WidgetInventory || !ItemSlotWidget)
	{
		WidgetInventory->DropButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		WidgetInventory->DropButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainCharacterPlayerController::CreateHUD()
{
	if(MainCharacterHUDWidget)
	{
		return;
	}

	if(!MainCharacterHUDClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing MainCharacterHUDClass. Fill in on the BP in PlayerController."), *FString(__FUNCTION__));
		return;
	}

	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if(!PS)
	{
		return;
	}

	MainCharacterHUDWidget = CreateWidget<UMainCharacterHUDWidget>(this, MainCharacterHUDClass);
	MainCharacterHUDWidget->AddToViewport();

	//Set the attributes
	MainCharacterHUDWidget->SetCurrentHealth(PS->GetHealth());
	MainCharacterHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	MainCharacterHUDWidget->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
	
	MainCharacterHUDWidget->SetCurrentResource(PS->GetResource());
	MainCharacterHUDWidget->SetMaxResource(PS->GetMaxResource());
	MainCharacterHUDWidget->SetResourcePercentage(PS->GetResource() / FMath::Max<float>(PS->GetMaxResource(), 1.f));

	MainCharacterHUDWidget->SetCurrentExperience(PS->GetExperience());
	MainCharacterHUDWidget->SetMaxExperience(PS->GetMaxExperience());
	MainCharacterHUDWidget->SetExperiencePercentage(PS->GetExperience() / FMath::Max<float>(PS->GetMaxExperience(), 1.f));

	MainCharacterHUDWidget->SetCharacterLevel(PS->GetCharacterLevel());

	DamageNumberClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/UI/PlayerWidgets/DamageNumbers/WC_DamageText.WC_DamageText_C"));
	if(!DamageNumberClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find DamageNumberClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}
	
}

UMainCharacterHUDWidget* AMainCharacterPlayerController::GetHUD()
{
	return MainCharacterHUDWidget;
}
//TODO Might get rid of it
void AMainCharacterPlayerController::ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter)
{
	AEnemyBase* EnemyBase;
	EnemyBase = Cast<AEnemyBase>(TargetCharacter);
	if(EnemyBase)
	{
		UPFloatingDamageNumberWComponent* DamageText = NewObject<UPFloatingDamageNumberWComponent>(EnemyBase, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount, true);
	}
}

void AMainCharacterPlayerController::ShowHealNumber(float HealAmount, AMainCharacter* MainCharacter)
{
	if(MainCharacter)
	{
		UPFloatingDamageNumberWComponent* HealText = NewObject<UPFloatingDamageNumberWComponent>(MainCharacter, DamageNumberClass);
		HealText->RegisterComponent();
		HealText->AttachToComponent(MainCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		HealText->SetDamageText(HealAmount, false);
	}
}

void AMainCharacterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AMainCharacterPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	CreateHUD();
}
