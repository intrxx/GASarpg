// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPCharacterMovementComponent();
	
	virtual float GetMaxSpeed() const override;
};
