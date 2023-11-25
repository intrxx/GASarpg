// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UGASAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedHealDelegate, UGASAbilitySystemComponent*, SourceASC, float, UnmitigatedHeal, float, MitigatedHeal);
class ATickTimestopActor;

UCLASS()
class PROJEKT_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool CharacterAbilitiesGiven = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ATickTimestopActor* TickActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsTimeFrozen = false;

	FReceivedDamageDelegate ReceiveDamage;
	FReceivedHealDelegate ReceiveHeal;

	virtual void ReceivedDamage(UGASAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
	virtual void ReceivedHeal(UGASAbilitySystemComponent* SourceASC, float UnmitigatedHeal, float MitigatedHeal);
};
