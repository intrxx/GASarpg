// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimestopWidget.generated.h"


class UVerticalBox;

UCLASS()
class PROJEKT_API UTimestopWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AddNewMoveToWidget(FString MoveName);

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;
};
