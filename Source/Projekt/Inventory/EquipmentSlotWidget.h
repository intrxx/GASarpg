// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotWidget.generated.h"

/**
 *
 * 
 */
class UItemObject;
class UImage;
class UButton;
class UTextBlock;

class AMainCharacter;
class AMainCharacterPlayerController;

class UGASGameplayAbility;
class UGameplayAbility;

UENUM()
enum EItemType
{
	NONE,
	MainHand,
	OffHand,
	Helmet,
	Breastplate,
};

UCLASS()
class PROJEKT_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void ButtonClicked();
	
	UFUNCTION(BlueprintCallable)
	void OnHover();
	
	UFUNCTION(BlueprintCallable)
	void OnUnhover();

	void RemoveItemFromSlot();
	void AddItemToSlot();
	void SwapItemsAround();

	void ChangeAbility(UItemObject* Item);
	void RestoreDefaultAbility(UItemObject* Item);

public:
	UPROPERTY(EditInstanceOnly)
	TEnumAsByte<EItemType> SlotType;
	
	UPROPERTY()
	AMainCharacterPlayerController* PC;
	UPROPERTY()
	AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadWrite)
	UItemObject* ItemObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* BackgroundTexture;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Thumbnail;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* EquipmentButton;

	UPROPERTY()
	int16 SlotIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> DefaultAbility;
};
