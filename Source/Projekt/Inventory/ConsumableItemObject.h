// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "ConsumableItemObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UConsumableItemObject : public UItemObject
{
	GENERATED_BODY()
	
protected:
	virtual void Use(AMainCharacter* MainCharacter) override; 
};
