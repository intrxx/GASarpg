// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupVolume.h"

#include "PopupBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APopupVolume::APopupVolume()
{
	VolumeTrigger = CreateDefaultSubobject<UBoxComponent>("VolumeTrigger");
	SetRootComponent(VolumeTrigger);
	
}

// Called when the game starts or when spawned
void APopupVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void APopupVolume::VolumeOverlap()
{
	if(PopupWidgetClass)
	{
		CreateWidget<UPopupBase>(GetWorld()->GetFirstPlayerController(), PopupWidgetClass)->AddToViewport();
	}

	Destroy();
}

void APopupVolume::UpdateVolumeSize()
{
	VolumeTrigger->SetBoxExtent(VolumeSize, true);
}


