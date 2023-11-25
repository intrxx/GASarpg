// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapOil.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projekt/AI/EnemyBase.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Projekt/Miscellaneous/EIGameplayTags.h"


ATrapOil::ATrapOil()
{
	FireVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire VFX"));
	FireVFX->SetupAttachment(SphereComponent);

	const FEIGameplayTags& GameplayTags = FEIGameplayTags::Get();
	BurningTag = GameplayTags.Debuff_Burning;
	OiledTag = GameplayTags.Debuff_Oiled;
}

void ATrapOil::IgniteOil()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Ignited oil c++"));
	if(FireVFX)
	{
		FireVFX->Activate();
	}
}

void ATrapOil::Test()
{
	InEditor();
}

void ATrapOil::Delete()
{
	InEditorDelete();
}

void ATrapOil::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

	if(MainCharacter)
	{
		if(MainCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(BurningTag))
		{
			IgniteOil();
		}
		else
		{
			MainCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(OiledTag);
			// TODO add oiled status effect
		}
		
		return;	
	}

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OtherActor);

	if(EnemyBase)
	{
		return;
	}

	if(OtherActor->ActorHasTag("Burning"))
	{
		IgniteOil();
	}
}
