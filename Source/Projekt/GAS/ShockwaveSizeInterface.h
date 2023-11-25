// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShockwaveSizeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UShockwaveSizeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJEKT_API IShockwaveSizeInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void ChangeAbility();
};
