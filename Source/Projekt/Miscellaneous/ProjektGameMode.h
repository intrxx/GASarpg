// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjektGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API AProjektGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AProjektGameMode();

	void MainCharacterDied(AController* Controller);
	
	
protected:
	virtual void BeginPlay() override;

	void RespawnMainCharacter(AController* Controller);
	
	TSubclassOf<class AMainCharacter> CharacterClass;

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;
	float RespawnDelay;
};
