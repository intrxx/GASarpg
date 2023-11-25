// Fill out your copyright notice in the Description page of Project Settings.


#include "TickTimestopActor.h"

#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.h"
#include "GA_Timestop.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "TimestopWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/PostProcessComponent.h"

// Sets default values
ATickTimestopActor::ATickTimestopActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NegativeFilterComponent = CreateDefaultSubobject<UPostProcessComponent>("NegativeFilterPostProcessComponent");
}

// Called when the game starts or when spawned
void ATickTimestopActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATickTimestopActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// For now we don't add max buffed in testing it didn't bloat RAM enough to warrant doing some weird
	// circular buffer, if someone wants to spend 10 hours to crash it idc
	if(bShouldRecord && TestMaxTransforms > -INFINITY)
	{
		// Adds the first one to fix crashing on index check
		if(TimestopAbility->Transforms.IsEmpty())
		{
			TimestopAbility->Transforms.Add(TimestopAbility->MainCharacter->GetActorTransform());
			TimeIndex++;
			return;
		}
		
		// Doesnt add the same transforms to make it smooth and to not bloat memory as much
		if(!TimestopAbility->Transforms[TimestopAbility->Transforms.Num() - 1].GetLocation().Equals(TimestopAbility->MainCharacter->GetActorLocation()))
		{
			TimestopAbility->Transforms.Add(TimestopAbility->MainCharacter->GetActorTransform());
			TimeIndex++;

			// Adding Move to UI
			if(TimestopAbility->AbilitiesArray.IsEmpty())
			{
				if(!TimestopWidget->VerticalBox->HasAnyChildren())
				{
					TimestopWidget->AddNewMoveToWidget("Move");
				}
			}
			else if(TimestopAbility->AbilitiesArray[TimestopAbility->AbilitiesArray.Num() - 1].TimeIndex == TimeIndex - 1)
			{
				TimestopWidget->AddNewMoveToWidget("Move");
			}
		}
		//TestMaxTransforms--;
	}
}

void ATickTimestopActor::StartRewind()
{
	MainCharacter->CustomTimeDilation = 10000.0f;

	NegativeFilterComponent->DestroyComponent();
	
	RewindTime();

	if(TrailVFX && MainCharacter)
	{
		NiagaraTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailVFX, MainCharacter->GetMesh(), NAME_None, FVector(0.f), FRotator(0.f),
			EAttachLocation::Type::KeepRelativeOffset, false);
		NiagaraTrailComponent->SetCustomTimeDilation(10000.0f);

		
	}
}

void ATickTimestopActor::RewindTime()
{
	// Checks if this rewind frame we used an ability if so execute it
	if(!TimestopAbility->AbilitiesArray.IsEmpty() && TimestopAbility->AbilitiesArray[0].TimeIndex == TimeIndex)
	{
		// Look for added activable ability and activate it
		if(MainCharacter->GetAbilitySystemComponent()->TryActivateAbility(TimestopAbility->AbilitiesArray[0].Ability->GetCurrentAbilitySpecHandle()))
		{
			// Removes text of ability from UI
			TimestopWidget->VerticalBox->RemoveChildAt(0);
			// Removes ability from the array
			TimestopAbility->AbilitiesArray.RemoveAt(0, 1, true);

			// Removes holograph VFX
			if(!HolographArray.IsEmpty())
			{
				HolographArray[0]->DestroyInstance();
				HolographArray.RemoveAt(0, 1, true);
			}

			// Deactivate rewind VFX 
			NiagaraTrailComponent->Deactivate();
		}
		else
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("FAILED TO ACTIVATE ABILITY TICKACTOR.CPP"));
			GetWorldTimerManager().SetTimer(RewindHandle, this, &ATickTimestopActor::RewindTime, UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * RewindTick);
		}
		return;
	}
	// If no ability used then move character
	if(!TimestopAbility->Transforms.IsEmpty())
	{
		MainCharacter->SetActorTransform(TimestopAbility->Transforms[0]);
		TimestopAbility->Transforms.RemoveAt(0, 1, true);

		// TODO this one casts in almost tick might hit performance on rewind
		if(TimestopWidget->VerticalBox->HasAnyChildren())
		{
			if(Cast<UTextBlock>(TimestopWidget->VerticalBox->GetChildAt(0))->Text.EqualTo(FText::FromString("Move")))
			{
				TimestopWidget->VerticalBox->RemoveChildAt(0);
				if(RunAnim)
				{
					MainCharacter->PlayAnimMontage(RunAnim);
				}
			}
		}
		TimeIndex++;
	}
	
	// We end rewind when all transform array is empty if not repeat
	if(!TimestopAbility->Transforms.IsEmpty())
	{
		GetWorldTimerManager().SetTimer(RewindHandle, this, &ATickTimestopActor::RewindTime, UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * RewindTick);
	}
	else
	{
		// To fix not activating ability if it's the last move
		if(!TimestopAbility->AbilitiesArray.IsEmpty())
		{
			if(MainCharacter->GetAbilitySystemComponent()->TryActivateAbility(TimestopAbility->AbilitiesArray[0].Ability->GetCurrentAbilitySpecHandle()))
			{
				TimestopWidget->VerticalBox->RemoveChildAt(0);
				
				TimestopAbility->AbilitiesArray.RemoveAt(0, 1, true);
				
				// Removes holograph VFX
				if(!HolographArray.IsEmpty())
				{
					HolographArray[0]->DestroyInstance();
					HolographArray.RemoveAt(0, 1, true);
				}
				
				NiagaraTrailComponent->Deactivate();
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("FAILED TO ACTIVATE ABILITY TICKACTOR.CPP"));
				GetWorldTimerManager().SetTimer(RewindHandle, this, &ATickTimestopActor::RewindTime, UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * RewindTick);
			}
		}
		// If no abilities left to rewind
		else
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			MainCharacter->CustomTimeDilation = 1.0f;

			GAS->bIsTimeFrozen = false;
			TimestopWidget->RemoveFromParent();

			NiagaraTrailComponent->Deactivate();
			
			TimestopAbility->OnEnd();

			MainCharacter->StopAnimMontage();

			Destroy();
		}
	}
}

void ATickTimestopActor::CancelLastMove()
{
	// Remove last move from UI
	TimestopWidget->VerticalBox->RemoveChildAt(TimestopWidget->VerticalBox->GetChildrenCount() - 1);
	
	// Check if last move was an ability if so remove it
	if(!TimestopAbility->AbilitiesArray.IsEmpty() && TimestopAbility->AbilitiesArray[TimestopAbility->AbilitiesArray.Num() - 1].TimeIndex == TimeIndex)
	{
		TimestopAbility->AbilitiesArray.RemoveAt(TimestopAbility->AbilitiesArray.Num() - 1, 1, true);

		// Removes holograph VFX
		if(!HolographArray.IsEmpty())
		{
			HolographArray[HolographArray.Num() - 1]->DestroyInstance();
			HolographArray.RemoveAt(HolographArray.Num() - 1, 1, true);
		}
	}
	// Remove last move until last ability
	else if(!TimestopAbility->AbilitiesArray.IsEmpty())
	{
		for(TimeIndex; TimeIndex > TimestopAbility->AbilitiesArray[TimestopAbility->AbilitiesArray.Num() - 1].TimeIndex; TimeIndex--)
		{
			TimestopAbility->Transforms.RemoveAt(TimestopAbility->Transforms.Num() - 1, 1, true);
		}
		
		MainCharacter->SetActorTransform(TimestopAbility->Transforms[TimeIndex - 1]);
	}
	// Remove all moves until begin timestop because we only moved at this point
	else if(TimestopAbility->AbilitiesArray.IsEmpty() && TimeIndex > 1)
	{
		MainCharacter->SetActorTransform(TimestopAbility->Transforms[0]);
		
		TimestopAbility->Transforms.Empty();
		TimeIndex = 0;
	}
	// We are at the beginning of timestop so we end ability
	else
	{
		TimestopAbility->OnEnd();
		Destroy();
	}
}

void ATickTimestopActor::AddTimestopWidget()
{
	if(TimestopWidgetClass)
	{
		TimestopWidget = Cast<UTimestopWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), TimestopWidgetClass));
		TimestopWidget->AddToViewport();
	}
}





