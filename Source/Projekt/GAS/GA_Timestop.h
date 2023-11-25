// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASGameplayAbility.h"
#include "TickTimestopActor.h"
#include "GA_Timestop.generated.h"

USTRUCT()
struct FAbility
{
	GENERATED_BODY()

	UPROPERTY()
	UGASGameplayAbility* Ability;

	UPROPERTY()
	int TimeIndex;
};

class AMainCharacter;
class ATickTimestopActor;
UCLASS()
class PROJEKT_API UGA_Timestop : public UGASGameplayAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	void OnEnd();

	// Setup references for functions
	UFUNCTION(BlueprintCallable)
	void ConfirmSetup(AActor* TickTimestopActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> Transforms;

	UPROPERTY(EditAnywhere)
	TArray<FAbility> AbilitiesArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATickTimestopActor* TickActor;


};
