// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Timestop.h"

#include "GASAbilitySystemComponent.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "NiagaraComponent.h"

void UGA_Timestop::OnEnd_Implementation()
{
	// THIS DOESNT GET CALLED
	TickActor->Destroy();
}

// For caching refs
void UGA_Timestop::ConfirmSetup(AActor* TickTimestopActor)
{
	// Timestop refs
	TickActor = Cast<ATickTimestopActor>(TickTimestopActor);

	// TickActor refs
	TickActor->TimestopAbility = this;
	TickActor->bShouldRecord = true;
	TickActor->MainCharacter = MainCharacter;
	TickActor->GAS = Cast<UGASAbilitySystemComponent>(MainCharacter->GetAbilitySystemComponent());

	// GAS refs
	TickActor->GAS->TickActor = TickActor;
	TickActor->GAS->bIsTimeFrozen = true;

	// MainCharacter refs
	MainCharacter->TimestopActor = TickActor;
	
	// Fixes not finishing timestop without committing any moves
	Transforms.Add(MainCharacter->GetActorTransform());
	TickActor->TimeIndex++;
	Transforms.Add(MainCharacter->GetActorTransform());
	TickActor->TimeIndex++;
}
