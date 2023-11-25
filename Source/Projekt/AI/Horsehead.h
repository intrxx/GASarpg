// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "Horsehead.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API AHorsehead : public AEnemyBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	AHorsehead(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void Attack(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void PossessedBy(AController* NewController) override;

protected:
	// GAS hard references
	UPROPERTY()
	class UGASAbilitySystemComponent* AbilitySystemComponentRef;
	UPROPERTY()
	class UGASAttributeSet* AttributeSetRef;
	
	FDelegateHandle HealthChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};

inline void AHorsehead::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}
