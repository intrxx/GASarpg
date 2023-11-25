// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PWeaponBase.generated.h"

UCLASS()
class PROJEKT_API APWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	FORCEINLINE UStaticMeshComponent* GetStaticWeapon() const { return StaticWeapon; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UStaticMeshComponent* StaticWeapon;
	
};
