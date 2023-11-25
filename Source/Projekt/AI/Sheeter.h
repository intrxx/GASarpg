// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Sheeter.generated.h"

struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class PROJEKT_API ASheeter : public AEnemyBase
{
	GENERATED_BODY()
public:
	ASheeter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void Attack(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	// GAS hard references
	UPROPERTY()
	UGASAbilitySystemComponent* AbilitySystemComponentRef;
	UPROPERTY()
	UGASAttributeSet* AttributeSetRef;
	
	FDelegateHandle HealthChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
