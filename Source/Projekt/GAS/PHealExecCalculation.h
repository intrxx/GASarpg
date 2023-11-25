// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PHealExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UPHealExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UPHealExecCalculation();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
