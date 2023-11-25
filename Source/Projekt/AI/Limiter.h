// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Limiter.generated.h"

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

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
