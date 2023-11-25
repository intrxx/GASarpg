// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Projekt/GAS/GASAbilitySystemComponent.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "Projekt/GAS/GASGameplayAbility.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Projekt/Miscellaneous/MyGameInstance.h"
#include "Projekt/Miscellaneous/EIGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projekt/Shared/PCharacterMovementComponent.h"

// Sets default values
AEnemyBase::AEnemyBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ACharacter::GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidget->SetupAttachment(GetCapsuleComponent());
	
	const FEIGameplayTags& GameplayTags = FEIGameplayTags::Get();
	DeadTag = GameplayTags.State_Dead;
	EffectRemoveOnDeathTag = GameplayTags.Effect_RemoveOnDeath;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Sets transform for AI to move only around specific area (IE: around first spawning point)
	OriginTransform = GetTransform();
	
	GetMesh()->SetCustomDepthStencilValue(254);
	
	RotateWidgetToPlayer();
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateWidgetToPlayer();

	//GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Black,FString::Printf(TEXT("%f"),Attributes->GetMaxHealth()));
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return EnemyAbilitySystemComponent.Get();
}

void AEnemyBase::InitializeAttributes()
{
	if (EnemyAbilitySystemComponent.IsValid() && DefaultAttributeEffect)
	{
		
		FGameplayEffectContextHandle EffectContext = EnemyAbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = EnemyAbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = EnemyAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), EnemyAbilitySystemComponent.Get());
		}
	}
}

void AEnemyBase::GiveAbilities()
{
	if( GetLocalRole() != ROLE_Authority || !EnemyAbilitySystemComponent.IsValid())
	{
		// || !EnemyAbilitySystemComponent->CharacterAbilitiesGiven
		return;
	}
	
	for (TSubclassOf<UGASGameplayAbility>& StartupAbility : DefaultAbilities)
	{
		EnemyAbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	EnemyAbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AEnemyBase::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
	
	GetMesh()->SetRenderCustomDepth(true);
}

void AEnemyBase::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	GetMesh()->SetRenderCustomDepth(false);
}

void AEnemyBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if(NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Enemy")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun.Enemy")));

		EnemyAbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

float AEnemyBase::GetHealth() const
{
	if(EnemyAttributeSetBase.IsValid())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange,  FString::Printf(TEXT("Attribute set on enemy is valid")));
		return EnemyAttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float AEnemyBase::GetMoveSpeed() const
{
	if(EnemyAttributeSetBase.IsValid())
	{
		return EnemyAttributeSetBase->GetMoveSpeed();
	}
	return 0.0f;
}

float AEnemyBase::GetMoveSpeedBaseValue() const
{
	if(EnemyAttributeSetBase.IsValid())
	{
		return EnemyAttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(EnemyAttributeSetBase.Get())->GetBaseValue();
	}
	return 0.0f;
}

bool AEnemyBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AEnemyBase::RotateWidgetToPlayer()
{
	// TODO Might do some more advanced calc here later but it will do for now
	HealthWidget->SetWorldRotation(FRotator(60.0f,180.0f,0.0f));
}

void AEnemyBase::RemoveCharacterAbilities()
{
	if(GetLocalRole() != ROLE_Authority || !EnemyAbilitySystemComponent.IsValid())
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : EnemyAbilitySystemComponent->GetActivatableAbilities())
	{
		if((Spec.SourceObject == this) && DefaultAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for(int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		EnemyAbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}
	
	EnemyAbilitySystemComponent->CharacterAbilitiesGiven = false;
}

void AEnemyBase::CheckQuestOnDeath_Implementation()
{
}

void AEnemyBase::Die()
{
	//GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,TEXT("Entered Die Function"));

	// There might be an issue later with getting resource back for hitting enemies that are in dying montage
	if(bIsDying) { return; }

	bIsDying = true;
	
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	CheckQuestOnDeath();

	DetachFromControllerPendingDestroy();
	
	// Removes enemy from GI array to not load again
	Cast<UMyGameInstance>(GetGameInstance())->EnemyArray.Remove(this);

	if(EnemyAbilitySystemComponent.IsValid())
	{
		EnemyAbilitySystemComponent->CancelAllAbilities();

		//TODO IF WE NEED TO CLEAR SOME TAGS LATER ON
		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = EnemyAbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		EnemyAbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	
	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
		
		//GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,TEXT("Entered DeathMontage"));
		//TODO need proper montage, for now character just disappear, Finish dying should be called somewhere else, it works for now tho
		GetWorldTimerManager().SetTimer(DelayEnemyRemovalTimer,this,&AEnemyBase::FinishDying,DeathMontage->GetPlayLength());
	}
	else
	{
		FinishDying();
	}
}

void AEnemyBase::FinishDying()
{
	Destroy();
}


