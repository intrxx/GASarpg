// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "WeaponItemObject.generated.h"

class UGASGameplayAbility;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class PROJEKT_API UWeaponItemObject : public UItemObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	virtual void Use(AMainCharacter* MainCharacter) override;

	UFUNCTION(BlueprintNativeEvent)
	void Equip(UGASGameplayAbility* Ability);

	UFUNCTION(BlueprintNativeEvent)
	void Unequip(UGASGameplayAbility* Ability);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 InputIndex;

};
