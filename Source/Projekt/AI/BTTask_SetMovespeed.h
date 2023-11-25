// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetMovespeed.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UBTTask_SetMovespeed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SetMovespeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTag")
	float MoveSpeed;
};
