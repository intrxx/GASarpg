// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "EIInputConfig.generated.h"


class UInputAction;
struct FGameplayTag;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTaggedInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
		const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
		FGameplayTag InputTag;
};


UCLASS()
class PROJEKT_API UEIInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
		TArray<FTaggedInputAction> TaggedInputActions;
};
