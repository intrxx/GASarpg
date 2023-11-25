// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PLevelChangeActor.generated.h"

UCLASS()
class PROJEKT_API APLevelChangeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APLevelChangeActor();

public:
	
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(FName LevelToLoad);
	
};
