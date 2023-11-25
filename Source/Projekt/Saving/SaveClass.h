// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

// Fixes issues with not compiling due to struct size
#include "Projekt/AI/EnemyBase.h"
#include "Projekt/Inventory/ChestBase.h"

#include "SaveClass.generated.h"

struct FChestData;
struct FEnemyData;
class UItemObject;
class APickup;

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY()

	// Save Transform
	UPROPERTY()
	FTransform PlayerTransform;

	UPROPERTY()
	TArray<TSubclassOf<UItemObject>> InventoryClassArray;
	UPROPERTY()
	TArray<TSubclassOf<UItemObject>> EquipmentClassArray;

	// For saving item info (resets to default otherwise)
	UPROPERTY()
	TArray<int8> InventoryIndex;
	UPROPERTY()
	TArray<int8> EquipmentIndex;
	UPROPERTY()
	TArray<int8> AmountIndex;
};

UCLASS()
class PROJEKT_API USaveClass : public USaveGame
{
	GENERATED_BODY()
	
public:
	USaveClass();
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FDateTime CreationTime;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString ScreenshotPath;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FPlayerData PlayerData;

	// Chest data
	UPROPERTY()
	TArray<FChestData> ChestArray;

	// Pickup data
	UPROPERTY()
	TArray<TSubclassOf<APickup>> PickupArray;

	UPROPERTY()
	TArray<FTransform> PickupTransformArray;

	UPROPERTY()
	TArray<FEnemyData> EnemyArray;
};
