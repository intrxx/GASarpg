// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavingWidget.generated.h"

class USaveClass;
class UVerticalBox;
class USaveSlotWidget;
class UTextBlock;
class UImage;
class UButton;

UCLASS()
class PROJEKT_API USavingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SaveName;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBoxSaveInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextDate;
	
	UPROPERTY(BlueprintReadWrite)
	USaveSlotWidget* SelectedSlot;

	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot1;
	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot2;
	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot3;
	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot4;
	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot5;
	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot6;
	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot7;
	UPROPERTY(meta = (BindWidget))
	USaveSlotWidget* SaveSlot8;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageSnapshot;
	UPROPERTY(meta = (BindWidget))
	UButton* DeleteButton;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MenuWidget;

	UFUNCTION(BlueprintCallable)
	void SavePressed();
};
