// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestWidget.h"

void UChestWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UChestWidget::DropButtonClicked()
{
}

UChestComponent* UChestWidget::GetInventoryComponent()
{
	return InventoryComponent;
}
