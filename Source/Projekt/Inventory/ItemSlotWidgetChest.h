// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidgetChest.generated.h"

class UItemObject;
class UImage;
class UButton;
class UTextBlock;

class UChestWidget;
class AMainCharacterPlayerController;

UCLASS()
class PROJEKT_API UItemSlotWidgetChest : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void ButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnHover();
	
	UFUNCTION(BlueprintCallable)
	void OnUnhover();

	void HandlePickUpInventoryItem();
	void HandlePutDownInventoryItem();
	void SwapSelectedItems();
	void HandleAddingItemToFreeSpot();

	UPROPERTY()
	AMainCharacterPlayerController* PC;
	UPROPERTY()
	UChestWidget* ChestWidget;

	UPROPERTY(BlueprintReadWrite)
	UItemObject* ItemObject;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UTexture2D* Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Thumbnail;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BGN;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* UseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AmountText;

	UPROPERTY()
	int16 SlotIndex;
};
