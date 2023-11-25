// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Projekt/AI/DefaultAIController.h"
#include "Projekt/AI/EnemyBase.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Projekt/MainCharacter/MainCharacterPlayerController.h"

UGASAttributeSet::UGASAttributeSet()
{

}

void UGASAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if(Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if(Attribute == GetMaxResourceAttribute())
	{
		AdjustAttributeForMaxChange(Resource, MaxResource, NewValue, GetResourceAttribute());
	}
}

void UGASAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTag = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);
	
	// Get owner
	//TODO Need to do this shit for both enemy and main character as we have different base classes ;-; and it's probably too late to change, lesson learned
	
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AEnemyBase* EnemyTargetCharacter = nullptr;
	AMainCharacter* MainTargetCharacter = nullptr;
	
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		
		EnemyTargetCharacter = Cast<AEnemyBase>(TargetActor);
		if(!EnemyTargetCharacter)
		{
			MainTargetCharacter = Cast<AMainCharacter>(TargetActor);
		}
	}
	
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AEnemyBase* EnemySourceCharacter = nullptr;
	AMainCharacter* MainSourceCharacter = nullptr;

	if(Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if(SourceController == nullptr && SourceActor != nullptr)
		{
			if(APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		if(SourceController)
		{
			EnemySourceCharacter = Cast<AEnemyBase>(SourceController->GetPawn());
		}
		else
		{
			EnemySourceCharacter = Cast<AEnemyBase>(SourceActor);
		}

		if(!EnemySourceCharacter)
		{
			if(SourceController)
			{
				MainSourceCharacter = Cast<AMainCharacter>(SourceController->GetPawn());
			}
			else
			{
				MainSourceCharacter = Cast<AMainCharacter>(SourceActor);
			}
		}
	}

	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		FHitResult HitResult;
		if(ContextHandle.GetHitResult())
		{
			HitResult = *ContextHandle.GetHitResult();
		}

		// Store a copy of the amount of damage done and clear the damage attribute
		const float DamageDone = GetDamage();
		SetDamage(0.0f);

		if(DamageDone > 0.0f)
		{
			bool WasAlive = true;
			if(MainTargetCharacter)
			{
				WasAlive = MainTargetCharacter->IsAlive();
			}
			else if(EnemyTargetCharacter)
			{
				WasAlive = EnemyTargetCharacter->IsAlive();
			}

			const float NewHealth = GetHealth() - DamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			if(MainTargetCharacter && WasAlive)
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s() %s Damage Received: %f"), TEXT(__FUNCTION__), *GetOwningActor()->GetName(), DamageDone);
				
				if(SourceActor != TargetActor)
				{
					AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(SourceController);
					if(PC)
					{
						if(EnemyTargetCharacter)
						{
							PC->ShowDamageNumber(DamageDone, EnemyTargetCharacter);
							UE_LOG(LogTemp, Warning, TEXT("Passing Damage Number in Main check: %f"), DamageDone);
						}
					}
				}
				
				if(!MainTargetCharacter->IsAlive())
				{
					//TODO Do all the  death logic here, like giving xp etc
					if(SourceController != TargetController)
					{
						
					}
				}
			}
			else if(EnemyTargetCharacter && WasAlive)
			{
				if(SourceActor != TargetActor)
				{
					ADefaultAIController* AIC = Cast<ADefaultAIController>(EnemyTargetCharacter->GetController());
					if(AIC)
					{
						if(EnemyTargetCharacter)
						{
							AIC->ShowDamageNumber(DamageDone, EnemyTargetCharacter);
							UE_LOG(LogTemp, Warning, TEXT("%s Damage Received: %f"), *GetOwningActor()->GetName(), DamageDone);
						}
					}
				}

				if(!EnemyTargetCharacter->IsAlive())
				{
					//TODO Do all the  death logic here, like giving xp etc
					if(SourceController != TargetController)
					{
						UE_LOG(LogTemp,Warning, TEXT("Try to give XP"));
						UGameplayEffect* GEExperience = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
						GEExperience->DurationPolicy = EGameplayEffectDurationType::Instant;

						int32 Idx = GEExperience->Modifiers.Num();
						GEExperience->Modifiers.SetNum(Idx + 1);
						
						FGameplayModifierInfo& InfoXP = GEExperience->Modifiers[Idx];
						InfoXP.ModifierMagnitude = FScalableFloat(GetExperience());
						InfoXP.ModifierOp = EGameplayModOp::Additive;
						InfoXP.Attribute = UGASAttributeSet::GetExperienceAttribute();

						UE_LOG(LogTemp, Warning, TEXT("XP amount given: %f"), GetExperience());

						Source->ApplyGameplayEffectToSelf(GEExperience, 1, Source->MakeEffectContext());
					}
				}
			}
		} 
	}
	else if(Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		const float HealDone = GetHeal();
		SetHeal(0);

		if(HealDone > 0.0f)
		{
			bool WasAlive = true;
			if(MainSourceCharacter)
			{
				WasAlive = MainSourceCharacter->IsAlive();
			}
			
			const float OldHealth = GetHealth();
			const float NewHealth = OldHealth + HealDone;
			
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			const float HealingDone = GetHealth() - OldHealth;
				
			if(MainSourceCharacter && WasAlive)
			{
				AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(SourceController);
				if(PC && HealingDone > 0)
				{
					PC->ShowHealNumber(HealingDone, MainSourceCharacter);
				}
			}
		}
	}
	else if (MainSourceCharacter && Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		const float CurrentXP = GetExperience();
		const float CurrentMaxXP = GetMaxExperience();
		UE_LOG(LogTemp, Warning, TEXT("XP: %f"), GetExperience());
		UE_LOG(LogTemp, Warning, TEXT("%s Owner Max XP: %f"), *GetOwningActor()->GetName(), GetMaxExperience());
		
		if(CurrentXP > CurrentMaxXP)
		{
			UE_LOG(LogTemp, Warning, TEXT("LEVEL UP"));
			const float XPOverMax = CurrentXP - CurrentMaxXP;
						
			SetExperience(XPOverMax);
			
			const float NewLevel = GetCharacterLevel()+1;
			SetCharacterLevel(NewLevel);
			
			const float NewMaxXP = GetMaxExperience() * 1.75f;
			SetMaxExperience(NewMaxXP);
		}
	}
	else if(Data.EvaluatedData.Attribute == GetResourceAttribute())
	{
		SetResource(FMath::Clamp(GetResource(), 0.0f, GetMaxResource()));
	}
	else if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Resource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxResource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ResourceRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Experience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxExperience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Damage, COND_None, REPNOTIFY_Always);
}

void UGASAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if(!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && ASC)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.0f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		/*
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Actor: %s, CurrentValue: %f"), *ASC->GetOwnerActor()->GetName(), CurrentValue));
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Actor: %s, MaxValue: %f"), *ASC->GetOwnerActor()->GetName(), CurrentMaxValue));
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Actor: %s, NewMaxValue: %f"), *ASC->GetOwnerActor()->GetName(), NewMaxValue));
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Actor: %s, NewDelta: %f"), *ASC->GetOwnerActor()->GetName(), NewDelta));
		*/
		ASC->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UGASAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Health, OldHealth);
}

void UGASAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, HealthRegen, OldHealthRegen);
}

void UGASAttributeSet::OnRep_Resource(const FGameplayAttributeData& OldResource)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Resource, OldResource);
}

void UGASAttributeSet::OnRep_ResourceRegen(const FGameplayAttributeData& OldResourceRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ResourceRegen, OldResourceRegen);
}

void UGASAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, AttackPower, OldAttackPower);
}

void UGASAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxHealth, OldMaxHealth);
}

void UGASAttributeSet::OnRep_MaxResource(const FGameplayAttributeData& OldMaxResource)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxResource, OldMaxResource);
}

void UGASAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, CharacterLevel, OldCharacterLevel);
}

void UGASAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Experience, OldExperience);
}

void UGASAttributeSet::OnRep_MaxExperience(const FGameplayAttributeData& OldMaxExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxExperience, OldMaxExperience);
}

void UGASAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UGASAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Damage, OldDamage);
}

void UGASAttributeSet::OnRep_DifficultyMultiplier(const FGameplayAttributeData& OldDifficultyMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, DifficultyMultiplier, OldDifficultyMultiplier);
}

void UGASAttributeSet::OnRep_Heal(const FGameplayAttributeData& OldHeal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Heal, OldHeal);
}




