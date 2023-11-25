// Fill out your copyright notice in the Description page of Project Settings.


#include "Limiter.h"

#include "AbilitySystemComponent.h"
#include "Projekt/GAS/GASAbilitySystemComponent.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "Projekt/UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"

ALimiter::ALimiter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponentRef = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponentRef->SetIsReplicated(true);
	AbilitySystemComponentRef->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	EnemyAbilitySystemComponent = AbilitySystemComponentRef;

	AttributeSetRef = CreateDefaultSubobject<UGASAttributeSet>(TEXT("AtributeSetBase"));

	EnemyAttributeSetBase = AttributeSetRef;
}

void ALimiter::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyAbilitySystemComponent.IsValid())
	{
		EnemyAbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		GiveAbilities();
	}

	// Delegate to handle enemy hp change
	HealthChangedDelegateHandle = EnemyAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EnemyAttributeSetBase->GetHealthAttribute()).AddUObject(this, &ALimiter::HealthChanged);

	EnemyAbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("EffectTag.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyBase::StunTagChanged);

}

void ALimiter::Attack(const FGameplayTag CallbackTag, int32 NewCount)
{
	EnemyAbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(CallbackTag));
}

void ALimiter::HealthChanged(const FOnAttributeChangeData& Data)
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
