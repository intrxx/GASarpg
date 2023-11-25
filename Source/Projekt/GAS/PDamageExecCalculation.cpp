// Fill out your copyright notice in the Description page of Project Settings.


#include "PDamageExecCalculation.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"

struct PDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DifficultyMultiplier);

	PDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASAttributeSet, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASAttributeSet, DifficultyMultiplier, Target, false);
	}
};

static const PDamageStatics& DamageStatics()
{
	static PDamageStatics DStatics;
	return DStatics;
}

UPDamageExecCalculation::UPDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DifficultyMultiplierDef);
}

void UPDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float DifficultyMultiplier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DifficultyMultiplierDef, EvaluateParameters, DifficultyMultiplier);
	DifficultyMultiplier = FMath::Max<float>(DifficultyMultiplier, 0.0f);

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluateParameters, Damage);
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.DamageToModify")), true, -1.0f),0.0f);
	
	float UnmitigatedDamage = Damage; //Can apply any damage boosters here

	float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + DifficultyMultiplier));

	if(MitigatedDamage > 0.0f)
	{
		
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));

		UGASAbilitySystemComponent* TargetASC = Cast<UGASAbilitySystemComponent>(TargetAbilitySystemComponent);
		if(TargetASC)
		{
			UGASAbilitySystemComponent* SourceASC = Cast<UGASAbilitySystemComponent>(SourceAbilitySystemComponent);
			TargetASC->ReceivedDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
		}
	}

}
