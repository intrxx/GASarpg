// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UBTTask_ActivateAbility : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ActivateAbility();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTag")
	FGameplayTag GameplayTag;
};
