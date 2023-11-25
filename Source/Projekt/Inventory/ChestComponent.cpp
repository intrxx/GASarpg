// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestComponent.h"

#include "ChestWidget.h"
#include "ItemObject.h"
#include "ItemSlotWidget.h"
#include "ItemSlotWidgetChest.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

// Sets default values for this component's properties
UChestComponent::UChestComponent()
{
	
}


// Called when the game starts
void UChestComponent::BeginPlay()
{
	Super::BeginPlay();

	OnChestUpdated.AddDynamic(this, &UChestComponent::UpdateInventory);
	
}

void UChestComponent::InitializeItemIcons()
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
		Item->ItemOwner = Chest;
	}
}

void UChestComponent::InitInventory()
{
	// Empties slot array references
	InventorySlotArray.Empty();
	SlotIndex = 0;

	// Empties created widgets from WrapBox
	ChestWidget->InventoryBox->ClearChildren();

	for(int i = 0; i < Capacity; i++)
	{
		UItemSlotWidgetChest* ItemSlotWidget = Cast<UItemSlotWidgetChest>(CreateWidget(GetWorld(), ItemSlotClass));

		if(ItemSlotWidget)
		{
			ChestWidget->InventoryBox->AddChildToWrapBox(ItemSlotWidget);
		
			ItemSlotWidget->SlotIndex = SlotIndex;
		
			SlotIndex++;
		
			InventorySlotArray.Add(ItemSlotWidget);
		}
	}
}

void UChestComponent::UpdateInventory()
{
	InitInventory();
	InitializeItemIcons();
}
