// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TickTimestopActor.generated.h"

class UGA_Timestop;
class AMainCharacter;
class UGASAbilitySystemComponent;
class UTimestopWidget;
class UNiagaraSystem;
class UNiagaraComponent;
class UPostProcessComponent;
class APostProcessVolume;

UCLASS()
class PROJEKT_API ATickTimestopActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATickTimestopActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartRewind();
	
	UFUNCTION(BlueprintCallable)
	void RewindTime();

	UFUNCTION(BlueprintCallable)
	void CancelLastMove();

	UFUNCTION(BlueprintCallable)
	void AddTimestopWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGA_Timestop* TimestopAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimestopWidget* TimestopWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTimestopWidget> TimestopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMainCharacter* MainCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldRecord = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGASAbilitySystemComponent* GAS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TestMaxTransforms = 10;

	UPROPERTY()
	FTimerHandle RewindHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RewindTick = 0.00001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AbilityIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* RunAnim;

	// VFX
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TrailVFX;

	UPROPERTY(BlueprintReadWrite)
	UNiagaraComponent* NiagaraTrailComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* HolographVFX;

	UPROPERTY(BlueprintReadWrite)
	UNiagaraComponent* NiagaraHolographComponent;

	UPROPERTY(BlueprintReadWrite)
	TArray<UNiagaraComponent*> HolographArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPostProcessComponent* NegativeFilterComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* NiagaraNegativeFilter;

	
};
