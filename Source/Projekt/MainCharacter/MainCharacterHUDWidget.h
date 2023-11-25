// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacterHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UMainCharacterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Events for player HUD
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowDeathScreen(bool ShowDS);
	
	//Attribute setters
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxResource(float MaxResource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentResource(float CurrentResource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResourcePercentage(float ResourcePercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentExperience(float Experience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxExperience(float MaxExperience);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetExperiencePercentage(float ExperiencePercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleSpellTree(bool bIsVisible);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCharacterLevel(int CharacterLevel);

	UPROPERTY(BlueprintReadWrite)
	bool bIsSkillTreeVisible = false;
	
};
