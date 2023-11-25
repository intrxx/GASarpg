// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UPlayerResourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UUPlayerResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHeathPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetResourcePercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetCharacterName(const FText& NewName);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleFloatingWidget(bool bIsVisible);
	
	UPROPERTY(BlueprintReadOnly)
	bool bWidgetIsVisible = true;
	
};
