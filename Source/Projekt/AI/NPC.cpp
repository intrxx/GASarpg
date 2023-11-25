// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Projekt/Dialog/Dialog.h"
#include "Projekt/Dialog/DialogWidget.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);

	DialogComponent = CreateDefaultSubobject<UDialog>(TEXT("Dialog Component"));

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UDialog* PlayerDialogComponent = MainCharacter->GetDialogComponent();

	if(!PlayerDialogComponent) { return; }
	
	if(!bIsTalking)
	{
		PlayerDialogComponent->SetTalkRangeStatus(true);
		PlayerDialog = FirstDialog;
		PlayerDialogComponent->SetAssociatedPawn(this);

		PlayerDialogComponent->ToggleTalking();
	}	
	else
	{
		PlayerDialogComponent->SetTalkRangeStatus(false);
		PlayerDialogComponent->SetAssociatedPawn(nullptr);
	}
}
