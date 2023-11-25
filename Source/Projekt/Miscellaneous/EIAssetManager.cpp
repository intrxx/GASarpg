// Fill out your copyright notice in the Description page of Project Settings.


#include "EIAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "EIGameplayTags.h"

UEIAssetManager::UEIAssetManager()
{
	
}

UEIAssetManager& UEIAssetManager::Get()
{
	check(GEngine);

	if(UEIAssetManager* EIAssetManager = Cast<UEIAssetManager>(GEngine->AssetManager))
	{
		return *EIAssetManager;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini, should be EIAssetManager"));

	return *NewObject<UEIAssetManager>();
}

void UEIAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FEIGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
