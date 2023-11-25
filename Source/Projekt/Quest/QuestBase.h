// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projekt/AI/EnemyBase.h"
#include "Projekt/Inventory/ItemObject.h"
#include "QuestBase.generated.h"


UENUM(BlueprintType)
enum class EQuestReward : uint8
{
	Default,
	Item,
	Experience
};

UENUM(BlueprintType)
enum class EClearCondition : uint8
{
	Default,
	Kill,
	Collect,
	Travel
};

USTRUCT(BlueprintType)
struct FReward
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestReward RewardType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemObject> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Experience;
};

USTRUCT(BlueprintType)
struct FObjective
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClearCondition ClearType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBase> EnemyToKill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemObject> ItemToCollect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberRequired;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ObjectiveID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int bIsComplete;
};

UCLASS(BlueprintType)
class PROJEKT_API UQuestBase : public UObject
{
	GENERATED_BODY()

public:
	UQuestBase();

	UFUNCTION(BlueprintCallable)
	void SetQuestDetails(FString NewName, FString NewDescription);

	UFUNCTION(BlueprintCallable)
	void SetObjective(int ObjectiveNumber, TSubclassOf<AEnemyBase> Enemy, TSubclassOf<UItemObject> Item, FString Description, int NumberRequired);

	UFUNCTION(BlueprintCallable)
	void SetNumberObjectives(int NumberObjectives);

	UFUNCTION(BlueprintCallable)
	void UpdateQuest();

	UFUNCTION(BlueprintCallable)
	void UpdateObjective(int ObjectiveNumber, int UpdateValue);

	UFUNCTION(BlueprintCallable)
	void FinishObjective(int ObjectiveNumber);

	UFUNCTION(BlueprintCallable)
	void FinishQuest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FReward Reward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjective> Objectives;
};
