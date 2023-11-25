// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckIfPlayerSeen.generated.h"

/**
 * 
 */
UCLASS()
class PROJEKT_API UBTService_CheckIfPlayerSeen : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CheckIfPlayerSeen();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
