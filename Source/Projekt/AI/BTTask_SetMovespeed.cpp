// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetMovespeed.h"

#include "AIController.h"
#include "DefaultAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyBase.h"

UBTTask_SetMovespeed::UBTTask_SetMovespeed()
{
	NodeName = TEXT("Set Pawn movespeed");
}

EBTNodeResult::Type UBTTask_SetMovespeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBase* Enemy = Cast<ADefaultAIController>(OwnerComp.GetAIOwner())->EnemyOwner;

	if(Enemy)
	{
		Enemy->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
