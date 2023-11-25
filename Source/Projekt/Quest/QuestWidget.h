// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

class UImage;
class UTextBlock;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class PROJEKT_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Text")
	FText SetQuestName();

	UFUNCTION(BlueprintPure, Category = "Text")
	FText SetQuestDescription();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* QuestNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* QuestDescriptionText;
};
