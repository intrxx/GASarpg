// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAIController.h"

#include "EnemyBase.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Projekt/UI/PFloatingDamageNumberWComponent.h"

ADefaultAIController::ADefaultAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	DamageNumberClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/UI/PlayerWidgets/DamageNumbers/WC_DamageText.WC_DamageText_C"));
	if(!DamageNumberClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find DamageNumberClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}
}

void ADefaultAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PerceptionComp = FindComponentByClass<UAIPerceptionComponent>();

	EnemyOwner = Cast<AEnemyBase>(InPawn);
	
	if(Blackboard && BehaviorTree)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}

	if(PerceptionComp)
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ADefaultAIController::OnStimulusChange);
	}
}

void ADefaultAIController::OnStimulusChange(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UpdatedActor);
	if(MainCharacter == nullptr) { return; }
	
	// We check if we enter the stimulus range or exit
	if(Stimulus.IsActive())
	{
		Blackboard->SetValueAsBool(FName(TEXT("bIsPlayerInRange")), true);
		Blackboard->SetValueAsObject(FName(TEXT("Player")), MainCharacter);
		
	}
	else
	{
		Blackboard->SetValueAsBool(FName(TEXT("bIsPlayerInRange")), false);
		Blackboard->ClearValue(FName(TEXT("Player")));	
	}
}

void ADefaultAIController::BeginPlay()
{
	Super::BeginPlay();

	if(BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void ADefaultAIController::ShowDamageNumber(float DamageAmount, AEnemyBase* TargetCharacter)
{
	if(TargetCharacter)
	{
		UPFloatingDamageNumberWComponent* DamageText = NewObject<UPFloatingDamageNumberWComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount, true);
	}
}
