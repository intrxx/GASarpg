// Fill out your copyright notice in the Description page of Project Settings.


#include "PHealExecCalculation.h"

#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"
#include "GASGameplayAbility.h"

struct FHealStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Heal);

	FHealStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASAttributeSet, Heal, Source, true);
	}
};

static const FHealStatics& HealStatics()
{
	static FHealStatics HealStatics;
	return HealStatics;
}

UPHealExecCalculation::UPHealExecCalculation()
{
	RelevantAttributesToCapture.Add(HealStatics().HealDef);
}

void UPHealExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;

	float Heal = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().HealDef, EvaluateParameters, Heal);
	Heal += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.HealToModify")), true, -1.0f), 0.0f);

	float UnbuffedHeal = Heal;
	
	float MitigatedHeal = UnbuffedHeal;

	if(UnbuffedHeal > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealStatics().HealProperty, EGameplayModOp::Additive, UnbuffedHeal));

		UGASAbilitySystemComponent* SourceASC = Cast<UGASAbilitySystemComponent>(SourceAbilitySystemComponent);
		if(SourceASC)
		{
			SourceASC->ReceivedHeal(SourceASC, UnbuffedHeal, MitigatedHeal);
		}
	}
}

