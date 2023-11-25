// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetInventory.h"

#include "ItemObject.h"
#include "ItemSlotWidget.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "MainCharacterInventory.h"
#include "Projekt/MainCharacter/MainCharacterPlayerController.h"


void UWidgetInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());
	MainCharacter = Cast<AMainCharacter>(PC->GetCharacter());
	
	// Sets this widget for inventory component due to callstack order
	InventoryComponent = MainCharacter->GetInventoryComponent();
	InventoryComponent->WidgetInventory = this;

	// Initializes inventory
	MainCharacter->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
}

void UWidgetInventory::DropButtonClicked()
{
	if(PC->ItemSlotWidget)
	{
		if(PC->ItemSlotWidget->ItemObject)
		{
			// to avoid rotating items add constraints to pickups in BP
			for (int i = 0; i < PC->ItemSlotWidget->ItemObject->Amount; i++)
			{
				PC->ItemSlotWidget->ItemObject->SpawnItem();
			}
			
			PC->RemoveItemSlotWidget();
		}
	}
}
