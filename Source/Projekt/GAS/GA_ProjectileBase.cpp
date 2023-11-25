// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_ProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGA_ProjectileBase::AGA_ProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void AGA_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}


