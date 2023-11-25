// Fill out your copyright notice in the Description page of Project Settings.


#include "PWeaponBase.h"

// Sets default values
APWeaponBase::APWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticWeaponMesh"));
	StaticWeapon->SetupAttachment(RootComponent);
	StaticWeapon->CastShadow = true;
	
}

// Called when the game starts or when spawned
void APWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}


