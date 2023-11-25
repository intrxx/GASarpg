// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterPlayerController.generated.h"

class AEnemyBase;
class UItemSlotWidget;
class UItemObject;
class UWidgetInventory;
class UChestWidget;
/**
 * 
 */
UCLASS()
class PROJEKT_API AMainCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainCharacterPlayerController();

	void CreateHUD();

	class UMainCharacterHUDWidget* GetHUD();

	// Damage number floating widget
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UPFloatingDamageNumberWComponent> DamageNumberClass;

	UFUNCTION()
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);

	UFUNCTION()
	void ShowHealNumber(float HealAmount, AMainCharacter* MainCharacter);
	
	// for drag and drop
	UPROPERTY()
	UItemSlotWidget* ItemSlotWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetInventory* WidgetInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "UI")
	UChestWidget* ChestWidget;
	
	void CreateItemSlotWidget(UItemObject* WidgetToCopy);
	void RemoveItemSlotWidget();

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UMainCharacterHUDWidget> MainCharacterHUDClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UMainCharacterHUDWidget* MainCharacterHUDWidget;
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
	
};
