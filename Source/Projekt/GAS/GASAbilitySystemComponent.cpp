// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAbilitySystemComponent.h"

void UGASAbilitySystemComponent::ReceivedDamage(UGASAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
	float MitigatedDamage)
{
	ReceiveDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void UGASAbilitySystemComponent::ReceivedHeal(UGASAbilitySystemComponent* SourceASC, float UnmitigatedHeal,
	float MitigatedHeal)
{
	ReceiveHeal.Broadcast(SourceASC, UnmitigatedHeal, MitigatedHeal);
}
