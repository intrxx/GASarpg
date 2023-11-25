// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestBase.h"

#include "ChestComponent.h"
#include "ChestWidget.h"
#include "ItemObject.h"
#include "ItemSlotWidget.h"
#include "Projekt/MainCharacter/MainCharacterPlayerController.h"


// Sets default values
AChestBase::AChestBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);

	ChestComponent = CreateDefaultSubobject<UChestComponent>(TEXT("ChestComponent"));

	SetRootComponent(Mesh);
}

void AChestBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

UChestComponent* AChestBase::GetChestComponent()
{
	return ChestComponent;
}

// Called when the game starts or when spawned
void AChestBase::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetCustomDepthStencilValue(252);
}

void AChestBase::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
	Mesh->SetRenderCustomDepth(true);
}

void AChestBase::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
	Mesh->SetRenderCustomDepth(false);
}

void AChestBase::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	if(ChestWidgetClass == nullptr) { return; }
	
	if(!bIsChestOpen)
	{
		bIsChestOpen = true;
		// Creating widget ref and adding to the screen
		ChestWidget = Cast<UChestWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), ChestWidgetClass));
		ChestWidget->AddToViewport();
		
		AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());

		PC->ChestWidget = ChestWidget;

		ChestWidget->InventoryComponent = ChestComponent;
		
		ChestComponent->ChestWidget = ChestWidget;

		ChestComponent->OnChestUpdated.Broadcast();
	}
	else 
	{
		// Removes widget from viewport

		AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());
		
		bIsChestOpen = false;
		PC->ChestWidget = nullptr;
		
		ChestWidget->RemoveFromViewport();
	}
}


