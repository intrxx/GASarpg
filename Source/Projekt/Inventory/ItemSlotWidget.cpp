// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"

#include "EquipmentSlotWidget.h"
#include "ItemObject.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "MainCharacterInventory.h"
#include "Projekt/MainCharacter/MainCharacterPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"



void UItemSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());
	MainCharacter = Cast<AMainCharacter>(PC->GetCharacter());

	BGN->SetBrushFromTexture(Background);
	Thumbnail->SetVisibility(ESlateVisibility::Collapsed); 
	AmountText->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemSlotWidget::ButtonClicked()
{
	// Pickup item without holding anything in PC
	if(!PC->ItemSlotWidget && MainCharacter && ItemObject)
	{
		HandlePickUpInventoryItem();
		OnUnhover();
	}

	// Click when holding item in PC
	else if(PC->ItemSlotWidget && MainCharacter)
	{
		HandlePutDownInventoryItem();
	}
}

void UItemSlotWidget::OnHover()
{
	if(!PC->ItemSlotWidget)
	{
		BGN->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
		Thumbnail->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));

		if(ItemObject)
		{
			for (auto EquipmentSlot : MainCharacter->GetInventoryComponent()->EquipmentSlotArray)
			{
				// Checks if hovered item type is same as equipment slot type
				if(EquipmentSlot->SlotType == ItemObject->ItemType)
				{
					EquipmentSlot->Background->SetColorAndOpacity(FLinearColor(0.0f, 0.3f, 0.0f));
					EquipmentSlot->Thumbnail->SetColorAndOpacity(FLinearColor(0.0f, 0.3f, 0.0f));

					EquipmentSlotWidget = EquipmentSlot;
				}
			}	
		}
	}
	else
	{
		BGN->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
		Thumbnail->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
	}
}

void UItemSlotWidget::OnUnhover()
{
	BGN->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
	Thumbnail->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));

	// Clean up currently highlighted item slot
	if(EquipmentSlotWidget)
	{
		EquipmentSlotWidget->Background->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
		EquipmentSlotWidget->Thumbnail->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
		
		EquipmentSlotWidget = nullptr;
	}
}

// Removes item to the grid and adds it to the PC to follow mouse
void UItemSlotWidget::HandlePickUpInventoryItem()
{
	PC->CreateItemSlotWidget(ItemObject);
	MainCharacter->GetInventoryComponent()->InventoryArray.RemoveAt(MainCharacter->GetInventoryComponent()->InventoryArray.Find(ItemObject));
	MainCharacter->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
}

void UItemSlotWidget::HandlePutDownInventoryItem()
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

			MainCharacter->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
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

void UItemSlotWidget::SwapSelectedItems()
{
	UItemObject* TempObject = DuplicateObject(ItemObject, nullptr);

	MainCharacter->GetInventoryComponent()->InventoryArray.Remove(ItemObject);
	ItemObject = PC->ItemSlotWidget->ItemObject;
	ItemObject->InventoryIndex = SlotIndex;
	MainCharacter->GetInventoryComponent()->InventoryArray.Add(ItemObject);
				
	PC->RemoveItemSlotWidget();
	PC->CreateItemSlotWidget(TempObject);
				
	MainCharacter->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
}

void UItemSlotWidget::HandleAddingItemToFreeSpot()
{
	MainCharacter->GetInventoryComponent()->InventoryArray.Add(PC->ItemSlotWidget->ItemObject);

	PC->ItemSlotWidget->ItemObject->InventoryIndex = SlotIndex;
	MainCharacter->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
	PC->RemoveItemSlotWidget();
}
