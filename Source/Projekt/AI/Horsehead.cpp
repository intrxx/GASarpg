// Fill out your copyright notice in the Description page of Project Settings.


#include "Horsehead.h"

#include "Projekt/GAS/GASAbilitySystemComponent.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "Projekt/UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AHorsehead::AHorsehead(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponentRef = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponentRef->SetIsReplicated(true);
	AbilitySystemComponentRef->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	EnemyAbilitySystemComponent = AbilitySystemComponentRef;

	AttributeSetRef = CreateDefaultSubobject<UGASAttributeSet>(TEXT("AtributeSetBase"));

	EnemyAttributeSetBase = AttributeSetRef;
	
}

void AHorsehead::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyAbilitySystemComponent.IsValid())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("ASC on Enemy is Valid")));
		EnemyAbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		GiveAbilities();
	}

	// Delegate to handle enemy hp change
	HealthChangedDelegateHandle = EnemyAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EnemyAttributeSetBase->GetHealthAttribute()).AddUObject(this, &AHorsehead::HealthChanged);

	EnemyAbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("EffectTag.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyBase::StunTagChanged);

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange,  FString::Printf(TEXT("%f"),EnemyAttributeSetBase->GetHealth()));
	/*
	for (TSubclassOf<UGASGameplayAbility>& StartupAbility : DefaultAbilities)
	{
		EnemyAbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}
	*/
	//	GetAbilitySystemComponent()->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("InputTag.Ability.Horsehead.Attack")),
	//		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHorsehead::Attack);
	
}

void AHorsehead::Attack(const FGameplayTag CallbackTag, int32 NewCount)
{
	EnemyAbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(CallbackTag));
}

void AHorsehead::HealthChanged(const FOnAttributeChangeData& Data)
{
	const float Health = Data.NewValue;
	
	// Updates floating healthbar with new value
	UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(HealthWidget->GetWidget());
	if (EnemyAttributeSetBase.IsValid())
	{
		HealthBarWidget->ProgressBar->SetPercent(Health / EnemyAttributeSetBase->GetMaxHealth());
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, UKismetStringLibrary::Conv_BoolToString(IsAlive()));

	if (!IsAlive())
	{
		Die();
	}
}
