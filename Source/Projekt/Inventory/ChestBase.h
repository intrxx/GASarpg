// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChestBase.generated.h"

class UChestWidget;
class UChestComponent;
class UItemObject;

USTRUCT()
struct FChestData
{
	GENERATED_BODY()

	// Save Inventory
	UPROPERTY()
	TArray<UItemObject*> InventoryArray;

	// For saving item info (resets to default otherwise)
	UPROPERTY()
	TArray<int8> InventoryIndex;
	UPROPERTY()
	TArray<int8> AmountIndex;
};

UCLASS()
class PROJEKT_API AChestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChestBase();

	virtual void Tick(float DeltaSeconds) override;

	UChestComponent* GetChestComponent();
	
	UPROPERTY()
	FChestData ChestData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginCursorOver() override;

	virtual void NotifyActorEndCursorOver() override;

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "UI")
	UChestComponent* ChestComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UChestWidget> ChestWidgetClass;

	UPROPERTY()
	UChestWidget* ChestWidget;

	UPROPERTY()
	bool bIsChestOpen;

};
