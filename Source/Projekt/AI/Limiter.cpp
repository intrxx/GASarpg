// Fill out your copyright notice in the Description page of Project Settings.


#include "Limiter.h"

#include "AbilitySystemComponent.h"
#include "Projekt/GAS/GASAbilitySystemComponent.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "Projekt/UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Components/TimelineComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"

ALimiter::ALimiter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponentRef = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponentRef->SetIsReplicated(true);
	AbilitySystemComponentRef->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	EnemyAbilitySystemComponent = AbilitySystemComponentRef;

	AttributeSetRef = CreateDefaultSubobject<UGASAttributeSet>(TEXT("AtributeSetBase"));
	EnemyAttributeSetBase = AttributeSetRef;
	
	DissolveTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComp"));
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
		// Start Dissolve Effect
		CreateDeathDynamicMaterialInstances();
		StartDissolve();

		HealthWidget->SetVisibility(false);
		Die();
	}
}

void ALimiter::UpdateDissolveMaterial(float DissolveValue)
{
	if(DissolveDynamicMaterialInstance_Body && DissolveDynamicMaterialInstance_Head)
	{
		DissolveDynamicMaterialInstance_Body->SetScalarParameterValue(TEXT("DissolveValue"), DissolveValue);
		DissolveDynamicMaterialInstance_Head->SetScalarParameterValue(TEXT("DissolveValue"), DissolveValue);
		UE_LOG(LogTemp, Warning, TEXT("Updating %f"), DissolveValue);
	}
}

void ALimiter::CreateDeathDynamicMaterialInstances()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateDeathDynamicMaterialInstances"));
	if(DissolveMaterialInstance_Body && DissolveMaterialInstance_Head)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateDeathDynamicMaterialInstances if "));
		DissolveDynamicMaterialInstance_Body = UMaterialInstanceDynamic::Create(DissolveMaterialInstance_Body, this);
		DissolveDynamicMaterialInstance_Head = UMaterialInstanceDynamic::Create(DissolveMaterialInstance_Head, this);

		GetMesh()->SetMaterial(0, DissolveDynamicMaterialInstance_Body);
		GetMesh()->SetMaterial(1, DissolveDynamicMaterialInstance_Head);

		DissolveDynamicMaterialInstance_Body->SetScalarParameterValue(TEXT("DissolveValue"), -0.55f);
		DissolveDynamicMaterialInstance_Head->SetScalarParameterValue(TEXT("DissolveValue"), -0.55f);
		
		DissolveDynamicMaterialInstance_Body->SetScalarParameterValue(TEXT("Glow"), 250.f);
		DissolveDynamicMaterialInstance_Head->SetScalarParameterValue(TEXT("Glow"), 250.f);
	}
}

void ALimiter::StartDissolve()
{
	DissolveTrackDelegate.BindDynamic(this, &ThisClass::UpdateDissolveMaterial);
	if(DissolveCurve && DissolveTimelineComp)
	{
		DissolveTimelineComp->AddInterpFloat(DissolveCurve, DissolveTrackDelegate);
		DissolveTimelineComp->Play();
	}
}
