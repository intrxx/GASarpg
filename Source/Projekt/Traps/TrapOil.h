// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TrapBase.h"
#include "TrapOil.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API ATrapOil : public ATrapBase
{
	GENERATED_BODY()

public:
	
	ATrapOil();
	
	// Function that ignites oil creating damaging AoE fire
	UFUNCTION(BlueprintCallable)
	void IgniteOil();

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UNiagaraComponent* FireVFX;

	UFUNCTION(CallInEditor, Category="Test")
	void Test();

	UFUNCTION(BlueprintImplementableEvent)
	void InEditor();

	UFUNCTION(CallInEditor, Category="Test")
	void Delete();

	UFUNCTION(BlueprintImplementableEvent)
	void InEditorDelete();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag BurningTag;
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag OiledTag;
};
