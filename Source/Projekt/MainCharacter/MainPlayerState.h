// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API AMainPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMainPlayerState();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	class UGASAttributeSet* GetAttributeSetBase() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|MainPlayerState")
	bool IsAlive() const;
	
	//Getters for attributes from GASAttributeSet
	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetResource() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxResource() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetHealthRegen() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetResourceRegen() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	int32 GetExperience() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	int32 GetMaxExperience() const;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMoveSpeed();
	
	/*
	 * 
	 */
protected:
	UPROPERTY()
	class UGASAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UGASAttributeSet* AttributeSetBase;
	
	FGameplayTag DeadTag;

	//Delegates for listening for change in attributes values
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ResourceChangedDelegateHandle;
	FDelegateHandle MaxResourceChangedDelegateHandle;
	FDelegateHandle HealthRegenChangedDelegateHandle;
	FDelegateHandle ResourceRegenChangedDelegateHandle;
	FDelegateHandle ExperienceChangedDelegateHandle;
	FDelegateHandle MaxExperienceChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ResourceChanged(const FOnAttributeChangeData& Data);
	virtual void MaxResourceChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenChanged(const FOnAttributeChangeData& Data);
	virtual void ResourceRegenChanged(const FOnAttributeChangeData& Data);
	virtual void ExperienceChanged(const FOnAttributeChangeData& Data);
	virtual void MaxExperienceChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
