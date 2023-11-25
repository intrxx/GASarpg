// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Blueprint/UserWidget.h"
#include "WidgetInventory.generated.h"

class UWrapBox;
class UCanvasPanel;
class UMainCharacterInventory;
class UEquipmentSlotWidget;
class UButton;
class UOverlay;

class AMainCharacter;
class AMainCharacterPlayerController;

UCLASS()
class PROJEKT_API UWidgetInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void DropButtonClicked();
	
	UPROPERTY()
	AMainCharacter* MainCharacter;
	UPROPERTY()
	AMainCharacterPlayerController* PC;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWrapBox* InventoryBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEquipmentSlotWidget* MainHandWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	UMainCharacterInventory* InventoryComponent;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DropButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayEquipment;
};
