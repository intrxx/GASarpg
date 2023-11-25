// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.h"
#include "GA_Timestop.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TickTimestopActor.h"
#include "TimestopWidget.h"

UGASGameplayAbility::UGASGameplayAbility()
{
	
}

bool UGASGameplayAbility::TimestopCheck()
{
	GAS = Cast<UGASAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(GAS->bIsTimeFrozen && GAS->TickActor->bShouldRecord)
	{
		if(GAS->TickActor)
		{
			FAbility Ability;
			Ability.Ability = this;
			Ability.TimeIndex = GAS->TickActor->TimeIndex;
		
			GAS->TickActor->TimestopAbility->AbilitiesArray.Add(Ability);
			GAS->TickActor->TimestopWidget->AddNewMoveToWidget(*Ability.Ability->GetName());

			// Probably refactor so ability holds info about it and about what frame to spawn
			// TODO: this VFX is very costly as it computes a lot vertices

			UNiagaraComponent* Holograph = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
				GAS->TickActor->HolographVFX, GAS->TickActor->MainCharacter->GetActorLocation(),
				GAS->TickActor->MainCharacter->GetActorRotation(),FVector(1), false, true);

			UNiagaraFunctionLibrary::OverrideSystemUserVariableSkeletalMeshComponent(Holograph,
	"User.SkeletalMeshInput", GAS->TickActor->MainCharacter->GetMesh());
			
			GAS->TickActor->HolographArray.Add(Holograph); 
			return true;
		}
	}
	return false;
}

bool UGASGameplayAbility::RewindCheck()
{
	GAS = Cast<UGASAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());

	if(GAS->bIsTimeFrozen)
	{
		return true;
	}
	return false;
}

void UGASGameplayAbility::ResumeRewind()
{
	GAS->TickActor->RewindTime();
}

void UGASGameplayAbility::CheckForRewind()
{
	if(GAS->bIsTimeFrozen && !GAS->TickActor->bShouldRecord)
	{
		GAS->TickActor->NiagaraTrailComponent->Activate();
		ResumeRewind();
	}
}
