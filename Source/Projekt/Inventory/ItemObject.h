// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

/**
 * 
 */

class UTexture2D;
class AMainCharacter;
class APickup;

enum EItemType;

UENUM()
enum EItemOwner
{
	NoOwner,
	CharacterInventory,
	CharacterEquipment,
	Chest
};

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJEKT_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UItemObject();
	
	virtual void Use(AMainCharacter* MainCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(AMainCharacter* MainCharacter);

	void SpawnItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<APickup> PickupData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<UItemObject> ItemData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TEnumAsByte<EItemType> ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText UseActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	uint8 InventoryIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	uint8 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	uint8 Amount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	uint8 MaxStackableAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TEnumAsByte<EItemOwner> ItemOwner;

	// Changing impulse to spawn items a bit randomly around certain place
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Params")
	float MaxXYOffset = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Params")
	float MinXYOffset = -500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Params")
	float MaxZOffset = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Params")
	float MinZOffset = 500.0f;

};
