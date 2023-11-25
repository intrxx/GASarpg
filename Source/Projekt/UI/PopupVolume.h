// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PopupVolume.generated.h"

class UBoxComponent;
class UPopupBase;

UCLASS()
class PROJEKT_API APopupVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APopupVolume();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void VolumeOverlap();

	UFUNCTION(CallInEditor, Category = "Popup")
	void UpdateVolumeSize();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Popup")
	UBoxComponent* VolumeTrigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Popup")
	FVector VolumeSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Popup")
	TSubclassOf<UPopupBase> PopupWidgetClass;


};
