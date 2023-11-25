// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

class USaveSlotWidget;
class USaveClass;
class AMainCharacter;
class AChestBase;


UCLASS()
class PROJEKT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SaveGame(USaveSlotWidget* SaveSlotWidget);
	void LoadGame(USaveSlotWidget* SaveSlotWidget);

	void LoadSaveInfo(USaveSlotWidget* SaveSlotWidget, UUserWidget* InfoWidget);

	virtual void Init() override;

	UMyGameInstance();

	UPROPERTY()
	AMainCharacter* MainCharacter;

	UPROPERTY()
	TArray<AActor*> ChestArray;

	UPROPERTY()
	TArray<AActor*> PickupArray;

	UPROPERTY()
	TArray<AActor*> EnemyArray;

protected:
	void SavePlayerData(USaveClass* Save);
	void LoadPlayerData(USaveClass* Save);

	void SaveChestData(USaveClass* Save);
	void LoadChestData(USaveClass* Save);

	void SavePickupData(USaveClass* Save);
	void LoadPickupData(USaveClass* Save);

	void SaveEnemyData(USaveClass* Save);
	void LoadEnemyData(USaveClass* Save);

	void LoadPauseHandle();

	UPROPERTY()
	FTimerHandle LoadPauseTimerHandle;
};
