// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidgetChest.h"

#include "ChestComponent.h"
#include "ChestWidget.h"
#include "ItemObject.h"
#include "ItemSlotWidget.h"
#include "Projekt/MainCharacter/MainCharacterPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemSlotWidgetChest::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());

	ChestWidget = PC->ChestWidget;

	BGN->SetBrushFromTexture(Background);
	Thumbnail->SetVisibility(ESlateVisibility::Collapsed); 
	AmountText->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemSlotWidgetChest::ButtonClicked()
{
	// Pickup item without holding anything in PC
	if(!PC->ItemSlotWidget && ChestWidget && ItemObject)
	{
		HandlePickUpInventoryItem();
	}

	// Click when holding item in PC
	else if(PC->ItemSlotWidget && ChestWidget)
	{
		HandlePutDownInventoryItem();
	}
}

void UItemSlotWidgetChest::OnHover()
{
	if(!PC->ItemSlotWidget)
	{
		BGN->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
		Thumbnail->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
	}
	else
	{
		BGN->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
		Thumbnail->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
	}
}

void UItemSlotWidgetChest::OnUnhover()
{
	BGN->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
	Thumbnail->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
}

void UItemSlotWidgetChest::HandlePickUpInventoryItem()
{
	PC->CreateItemSlotWidget(ItemObject);
	ChestWidget->GetInventoryComponent()->InventoryArray.RemoveAt(ChestWidget->GetInventoryComponent()->InventoryArray.Find(ItemObject));
	ChestWidget->GetInventoryComponent()->OnChestUpdated.Broadcast();
}

void UItemSlotWidgetChest::HandlePutDownInventoryItem()
{	
	// If Click on occupied slot
	if(ItemObject)
	{
		// Check if you can stack item in PC with clicked item
		if(PC->ItemSlotWidget->ItemObject->ItemName.EqualTo(ItemObject->ItemName) && ItemObject->bIsStackable)
		{
			// Check if it doesnt exceed max amount stackable
			if(ItemObject->Amount + PC->ItemSlotWidget->ItemObject->Amount > ItemObject->MaxStackableAmount)
			{
				uint8 AmountToAdd = ItemObject->MaxStackableAmount - ItemObject->Amount;

				ItemObject->Amount += AmountToAdd;

				PC->ItemSlotWidget->ItemObject->Amount -= AmountToAdd;
				PC->ItemSlotWidget->AmountText->SetText(FText::AsNumber(PC->ItemSlotWidget->ItemObject->Amount));
			}
			else
			{
				ItemObject->Amount += PC->ItemSlotWidget->ItemObject->Amount;
				PC->RemoveItemSlotWidget();
			}
			ChestWidget->GetInventoryComponent()->OnChestUpdated.Broadcast();
		}
		// If different item swap places
		else
		{
			SwapSelectedItems();
		}
	}

	// Add item to not occupied slot
	else
	{
		HandleAddingItemToFreeSpot();
	}
}

void UItemSlotWidgetChest::SwapSelectedItems()
{
	UItemObject* TempObject = DuplicateObject(ItemObject, nullptr);
	
	ChestWidget->GetInventoryComponent()->InventoryArray.Remove(ItemObject);
	ItemObject = PC->ItemSlotWidget->ItemObject;
	ItemObject->InventoryIndex = SlotIndex;
	ChestWidget->GetInventoryComponent()->InventoryArray.Add(ItemObject);
				
	PC->RemoveItemSlotWidget();
	PC->CreateItemSlotWidget(TempObject);
				
	ChestWidget->GetInventoryComponent()->OnChestUpdated.Broadcast();
}

void UItemSlotWidgetChest::HandleAddingItemToFreeSpot()
{
	ChestWidget->GetInventoryComponent()->InventoryArray.Add(PC->ItemSlotWidget->ItemObject);

	PC->ItemSlotWidget->ItemObject->InventoryIndex = SlotIndex;
	ChestWidget->GetInventoryComponent()->OnChestUpdated.Broadcast();
	PC->RemoveItemSlotWidget();
}
