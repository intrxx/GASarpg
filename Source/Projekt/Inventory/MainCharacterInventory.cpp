// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterInventory.h"

#include "EquipmentSlotWidget.h"
#include "ItemObject.h"
#include "ItemSlotWidget.h"
#include "WidgetInventory.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"


// Sets default values for this component's properties
UMainCharacterInventory::UMainCharacterInventory()
{
}

// Called when the game starts
void UMainCharacterInventory::BeginPlay()
{
	Super::BeginPlay();

	OnInventoryUpdated.AddDynamic(this, &UMainCharacterInventory::UpdateInventory);
}

void UMainCharacterInventory::UpdateInventory()
{
	InitInventory();
	InitEquipment();
	InitializeItemIcons();
}

void UMainCharacterInventory::InitEquipment()
{
	EquipmentSlotArray.Empty();
	
	for(uint8 i = 1; i <= Equipment.NumberOfEquipmentSlots; i++)
	{
		UEquipmentSlotWidget* EquipmentSlotWidget = Cast<UEquipmentSlotWidget>(WidgetInventory->OverlayEquipment->GetChildAt(i));
		
		// Subtracts 1 from index to not count background image
		EquipmentSlotWidget->SlotIndex = i - 1;
		
		EquipmentSlotArray.Add(EquipmentSlotWidget);
	}

	if(EquipmentArray.IsEmpty()) { return; }

	for (auto Item : EquipmentArray)
	{
		EquipmentSlotArray[Item->InventoryIndex]->Thumbnail->SetBrushFromTexture(Item->Thumbnail);
		EquipmentSlotArray[Item->InventoryIndex]->Thumbnail->SetVisibility(ESlateVisibility::Visible);
		EquipmentSlotArray[Item->InventoryIndex]->ItemObject = Item;
		
		EquipmentSlotArray[Item->InventoryIndex]->EquipmentButton->SetToolTipText(FText::Format(NSLOCTEXT("English","English",
			"Press to: {Action Text}\nName: {Item Name}\nValue: {Value}\n{Description}"),
			Item->UseActionText,
			Item->ItemName,
			Item->ItemValue,
			Item->ItemDescription)
			);

		Item->ItemOwner = CharacterEquipment;
	}
}

// Sets icons visibility for added inventory items
void UMainCharacterInventory::InitializeItemIcons()
{
	if(InventoryArray.IsEmpty()) { return; }
	
	for (auto Item : InventoryArray)
	{
		InventorySlotArray[Item->InventoryIndex]->Thumbnail->SetBrushFromTexture(Item->Thumbnail);
		InventorySlotArray[Item->InventoryIndex]->Thumbnail->SetVisibility(ESlateVisibility::Visible);
		InventorySlotArray[Item->InventoryIndex]->ItemObject = Item;

		if(Item->Amount > 1)
		{
			InventorySlotArray[Item->InventoryIndex]->AmountText->SetText(FText::AsNumber(Item->Amount));
			InventorySlotArray[Item->InventoryIndex]->AmountText->SetVisibility(ESlateVisibility::Visible);
		}

		InventorySlotArray[Item->InventoryIndex]->UseButton->SetToolTipText(FText::Format(NSLOCTEXT("English","English",
			"Press to: {Action Text}\nName: {Item Name}\nValue: {Value}\n{Description}"),
			Item->UseActionText,
			Item->ItemName,
			Item->ItemValue,
			Item->ItemDescription)
			);
		Item->ItemOwner = CharacterInventory;
	}
}

void UMainCharacterInventory::InitInventory()
{
	// Empties slot array references
	InventorySlotArray.Empty();
	SlotIndex = 0;

	// Empties created widgets from WrapBox
	WidgetInventory->InventoryBox->ClearChildren();

	for(int i = 0; i < Capacity; i++)
	{
		UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>(CreateWidget(GetWorld(), ItemSlotClass));
		
		WidgetInventory->InventoryBox->AddChildToWrapBox(ItemSlotWidget);

		ItemSlotWidget->SlotIndex = SlotIndex;
		
		SlotIndex++;
		
		InventorySlotArray.Add(ItemSlotWidget);
	}
}

// Checks if inventory isnt full and adds item
// TODO Check if items can stack and if so stack them
bool UMainCharacterInventory::AddItem(UItemObject* Item)
{
	if(InventoryArray.Num() >= Capacity || !Item) { return false; }
	
	for(int i = 0; i < Capacity; i++)
	{
		bool bSpaceEmpty = true;
		
		for (UItemObject* ItemInArray : InventoryArray)
		{
			if(ItemInArray->ItemName.EqualTo(Item->ItemName) && Item->bIsStackable && ItemInArray->Amount != ItemInArray->MaxStackableAmount)
			{
				ItemInArray->Amount++;

				OnInventoryUpdated.Broadcast();

				return true;
			}
			
			if(ItemInArray->InventoryIndex == i)
			{
				bSpaceEmpty = false;
			}
		}
		
		if(bSpaceEmpty)
		{
			Item->InventoryIndex = i;
			break;
		}
	}
	
	InventoryArray.Add(Item);

	OnInventoryUpdated.Broadcast();

	return true;
}

bool UMainCharacterInventory::RemoveItem(UItemObject* Item)
{
	if(Item)
	{
		if(Item->Amount == 1)
		{
			InventoryArray.RemoveAt(InventoryArray.Find(Item));
		}
		else
		{
			Item->Amount--;
		}
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}
