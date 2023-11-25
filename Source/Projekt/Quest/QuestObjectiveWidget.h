// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestObjectiveWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJEKT_API UQuestObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ObjectiveDescriptionText;
};
