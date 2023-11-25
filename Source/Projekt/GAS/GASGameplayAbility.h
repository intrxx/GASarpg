// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projekt/Projekt.h"
#include "Abilities/GameplayAbility.h"
#include "GASGameplayAbility.generated.h"

class ATickTimestopActor;
class UGASAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAbilityDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Ability Details")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Ability Details")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Ability Details")
	FString InputName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Ability Details")
	float Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Ability Details")
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Ability Details")
	UTexture2D* Icon;
};

UCLASS()
class PROJEKT_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGASGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
	
	// For checking if we are in timestop
	UFUNCTION(BlueprintCallable)
	bool TimestopCheck();

	// For checking if we are in rewind
	UFUNCTION(BlueprintCallable)
	bool RewindCheck();

	UFUNCTION(BlueprintCallable)
	void ResumeRewind();

	UFUNCTION(BlueprintCallable)
	void CheckForRewind();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UGASAbilitySystemComponent* GAS;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAbilityDetails AbilityDetails;
};
