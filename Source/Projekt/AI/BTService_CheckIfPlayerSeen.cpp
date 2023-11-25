// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckIfPlayerSeen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckIfPlayerSeen::UBTService_CheckIfPlayerSeen()
{
	NodeName = "Update Player Location If Seen";
}

void UBTService_CheckIfPlayerSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if(PlayerPawn == nullptr) { return; }

	if(OwnerComp.GetAIOwner() == nullptr){ return; }
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),PlayerPawn->GetActorLocation());
}

