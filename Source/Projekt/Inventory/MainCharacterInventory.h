// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCharacterInventory.generated.h"

class APickup;
class UItemObject;
class UItemSlotWidget;
class UWidgetInventory;
class UEquipmentSlotWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

USTRUCT()
struct FEquipment
{
	GENERATED_BODY()
	UObject* MainHand = nullptr;
	
	uint8 NumberOfEquipmentSlots = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJEKT_API UMainCharacterInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMainCharacterInventory();

	bool AddItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UItemObject* Item);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UItemObject*> InventoryArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<UItemSlotWidget*> InventorySlotArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UItemObject*> EquipmentArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<UEquipmentSlotWidget*> EquipmentSlotArray;

	UPROPERTY()
	UWidgetInventory* WidgetInventory;

	UPROPERTY()
	uint16 SlotIndex = 0;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InitializeItemIcons();

	UFUNCTION(BlueprintCallable)
	void InitInventory();

	UFUNCTION()
	void UpdateInventory();

	UFUNCTION()
	void InitEquipment();

	//140
	UPROPERTY(BlueprintReadOnly)
	uint8 Capacity = 70;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemSlotWidget> ItemSlotClass;

	UPROPERTY()
	FEquipment Equipment;
};
