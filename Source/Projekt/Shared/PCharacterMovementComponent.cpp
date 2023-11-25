// Fill out your copyright notice in the Description page of Project Settings.


#include "PCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "Projekt/AI/EnemyBase.h"
#include "Projekt/MainCharacter/MainCharacter.h"

UPCharacterMovementComponent::UPCharacterMovementComponent()
{
	
}

float UPCharacterMovementComponent::GetMaxSpeed() const
{
	AMainCharacter* Owner = Cast<AMainCharacter>(GetOwner());
	AEnemyBase* AIOwner = Cast<AEnemyBase>(GetOwner());
	
	if(Owner)
	{
		if(!Owner->IsAlive())
		{
			return 0.0f;	
		}

		if(Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("EffectTag.Debuff.Stun"))))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Stunned"));
			return 0.0f;
		}
		
		return Owner->GetMoveSpeed();
	}

	if(AIOwner)
	{
		if(!AIOwner->IsAlive())
		{
			return 0.0f;	
		}

		if(AIOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("EffectTag.Debuff.Stun"))))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy Stunned"));
			return 0.0f;
		}
		
		return AIOwner->GetMoveSpeed();
	}

	return Super::GetMaxSpeed();
}

