// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ActivateAbility.h"

#include "DefaultAIController.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Projekt/GAS/GASAbilitySystemComponent.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	NodeName = TEXT("Activate Ability");
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AEnemyBase* Enemy = Cast<ADefaultAIController>(OwnerComp.GetAIOwner())->EnemyOwner;
	
	if(Enemy)
	{
		if(Enemy->GetAbilitySystemComponent())
		{
			Enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayTag), true);
			
			return EBTNodeResult::Succeeded;	
		}
	}
	return  EBTNodeResult::Failed;
}
