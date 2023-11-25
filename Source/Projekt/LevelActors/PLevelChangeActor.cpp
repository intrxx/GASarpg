// Fill out your copyright notice in the Description page of Project Settings.


#include "PLevelChangeActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APLevelChangeActor::APLevelChangeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APLevelChangeActor::ChangeLevel(FName LevelToLoad)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
}


