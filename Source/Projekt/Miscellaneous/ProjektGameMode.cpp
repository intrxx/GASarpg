// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjektGameMode.h"
#include "Projekt/MainCharacter/CharacterPaladin.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Projekt/MainCharacter/MainCharacterHUDWidget.h"
#include "Projekt/MainCharacter/MainCharacterPlayerController.h"
#include "Projekt/MainCharacter/MainPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"

AProjektGameMode::AProjektGameMode()
{
	RespawnDelay = 5.f;

	/*
	CharacterClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/Characters/BP_MainCharacter.BP_MainCharacter"));
	if (!CharacterClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find CharacterClass. If it was moved or changed, please update the reference location in C++."), *FString(__FUNCTION__));
	}
	*/
}

void AProjektGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AProjektGameMode::MainCharacterDied(AController* Controller)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(Controller);
	if(!PC)
	{
		return;
	}
	
	ACharacterPaladin* Hero = Cast<ACharacterPaladin>(PC->GetCharacter());
	if(!Hero)
	{
		return;	
	}
	
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Hero->GetCamera()->GetComponentTransform(), SpawnParams);
	Controller->UnPossess();
	Controller->Possess(SpectatorPawn);
	SpectatorPawn->TurnOff();

	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->ShowDeathScreen(true);
		}
	}
	
	RespawnDelegate = FTimerDelegate::CreateUObject(this, &AProjektGameMode::RespawnMainCharacter, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
}

void AProjektGameMode::RespawnMainCharacter(AController* Controller)
{
	/*
	AActor* PlayerStart = FindPlayerStart(Controller);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACharacterPaladin* CharacterPaladin = GetWorld()->SpawnActor<ACharacterPaladin>(CharacterClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

	APawn* OldSpectatorPawn = Controller->GetPawn();
	Controller->UnPossess();
	OldSpectatorPawn->Destroy();
	Controller->Possess(CharacterPaladin);

	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(Controller);
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->ShowDeathScreen(false);
		}
	}
	*/
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("PreviewLevel")));
}



