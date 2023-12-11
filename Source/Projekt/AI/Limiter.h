// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Components/TimelineComponent.h"
#include "Limiter.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInstance;
class UCurveFloat;
struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class PROJEKT_API ALimiter : public AEnemyBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	ALimiter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void Attack(const FGameplayTag CallbackTag, int32 NewCount);

//	virtual void PossessedBy(AController* NewController) override;

protected:
	// GAS hard references
	UPROPERTY()
	UGASAbilitySystemComponent* AbilitySystemComponentRef;
	UPROPERTY()
	UGASAttributeSet* AttributeSetRef;
	
	FDelegateHandle HealthChangedDelegateHandle;
	
	/**
	 *	Dissolve Effect
	 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimelineComp;

	FOnTimelineFloat DissolveTrackDelegate;
	
	UPROPERTY(EditAnywhere, Category = "Death|Dissolve")
	TObjectPtr<UCurveFloat> DissolveCurve;
	
	// Dynamic instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category = "Death|Dissolve")
	TObjectPtr<UMaterialInstanceDynamic> DissolveDynamicMaterialInstance_Body;

	UPROPERTY(VisibleAnywhere, Category = "Death|Dissolve")
	TObjectPtr<UMaterialInstanceDynamic> DissolveDynamicMaterialInstance_Head;

	// Material instance set on the Blueprint, used with the dynamic material instance
	UPROPERTY(EditAnywhere, Category = "Death|Dissolve")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance_Body;
	
	UPROPERTY(EditAnywhere, Category = "Death|Dissolve")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance_Head;

	/**
	 *	
	 */

protected:
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void CreateDeathDynamicMaterialInstances();
	void StartDissolve();
	
};
