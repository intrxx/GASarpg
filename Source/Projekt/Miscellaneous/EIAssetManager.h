// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "EIAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UEIAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UEIAssetManager();

	// Returns the AssetManager singleton object.
	static UEIAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
