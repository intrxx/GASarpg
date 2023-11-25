// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemObject.h"

void UConsumableItemObject::Use(AMainCharacter* MainCharacter)
{
	Super::Use(MainCharacter);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("ConsumableItemUsed"));
}
