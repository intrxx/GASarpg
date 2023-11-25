// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestWidget.generated.h"

class AMainCharacter;
class AMainCharacterPlayerController;
class UWrapBox;
class UChestComponent;
class UButton;

UCLASS()
class PROJEKT_API UChestWidget : public UUserWidget
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	UChestComponent* InventoryComponent;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DropButton;

	UFUNCTION()
	UChestComponent* GetInventoryComponent();
};
