// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PFloatingDamageNumberWComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UPFloatingDamageNumberWComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bIsDamaging);
};
