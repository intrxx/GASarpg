// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChestComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChestUpdated);

class UItemObject;
class UItemSlotWidgetChest;
class UChestWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJEKT_API UChestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChestComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnChestUpdated OnChestUpdated;

	UPROPERTY()
	UChestWidget* ChestWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UItemObject*> InventoryArray;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InitializeItemIcons();

	UFUNCTION(BlueprintCallable)
	void InitInventory();

	UFUNCTION()
	void UpdateInventory();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<UItemSlotWidgetChest*> InventorySlotArray;
	
	uint16 SlotIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	uint8 Capacity = 70;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemSlotWidgetChest> ItemSlotClass;
};
