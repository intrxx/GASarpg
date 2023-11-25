// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "DefaultAIController.generated.h"


class AEnemyBase;
class UBehaviorTreeComponent;
UCLASS()
class PROJEKT_API ADefaultAIController : public AAIController
{
	GENERATED_BODY()
public:
	ADefaultAIController();

	UPROPERTY(BlueprintReadWrite)
	AEnemyBase* EnemyOwner;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UPFloatingDamageNumberWComponent> DamageNumberClass;

	UFUNCTION()
	void ShowDamageNumber(float DamageAmount, AEnemyBase* TargetCharacter);
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnStimulusChange(AActor* UpdatedActor, struct FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UAIPerceptionComponent* PerceptionComp;
};
