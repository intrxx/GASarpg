// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSlotWidget.generated.h"

/**
 * 
 */

class USaveGame;
class UTexture2D;

UCLASS()
class PROJEKT_API USaveSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
    void ButtonPressed();

	UPROPERTY(EditInstanceOnly, Category = "Save")
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, Category = "Save")
	UUserWidget* OwningWidget;

	UPROPERTY(EditAnywhere, Category = "Save")
	UTexture2D* DefaultThumbnail;

protected:
	UFUNCTION(BlueprintPure, Category = "Text")
	FText SlotName();

	UPROPERTY(EditDefaultsOnly, Category = "Save")
	bool bShouldSave = false;

	UPROPERTY(EditDefaultsOnly, Category = "Save")
	bool bShouldLoad = false;
};

