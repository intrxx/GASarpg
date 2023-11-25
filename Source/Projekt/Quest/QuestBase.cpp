// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBase.h"

#include "Projekt/AI/EnemyBase.h"
#include "Projekt/Inventory/ItemObject.h"

UQuestBase::UQuestBase()
{
	Name = "No Name override!";
	Description = "No description override!";

	Reward.RewardType = EQuestReward::Default;
	Reward.Experience = 100.0f;
	Reward.Item = nullptr;
}

void UQuestBase::SetQuestDetails(FString NewName, FString NewDescription)
{
	Name = NewName;
	Description = NewDescription;
}

void UQuestBase::SetObjective(int ObjectiveNumber, TSubclassOf<AEnemyBase> Enemy, TSubclassOf<UItemObject> Item,
	FString NewDescription, int NumberRequired)
{
	if(ObjectiveNumber > Objectives.Num()) { return; }

	if(Enemy != nullptr)
	{
		Objectives[ObjectiveNumber].ClearType = EClearCondition::Kill;
		Objectives[ObjectiveNumber].EnemyToKill = Enemy;
	}
	else if(Item != nullptr)
	{
		Objectives[ObjectiveNumber].ClearType = EClearCondition::Collect;
		Objectives[ObjectiveNumber].ItemToCollect = Item;
	}

	Objectives[ObjectiveNumber].Description = NewDescription;
	Objectives[ObjectiveNumber].NumberRequired = NumberRequired;
	Objectives[ObjectiveNumber].ObjectiveID = ObjectiveNumber;
}

void UQuestBase::SetNumberObjectives(int NumberObjectives)
{
	Objectives.SetNum(NumberObjectives);
}

void UQuestBase::UpdateQuest()
{
	
}

void UQuestBase::UpdateObjective(int ObjectiveNumber, int UpdateValue)
{
	if(ObjectiveNumber > Objectives.Num()) { return; }

	Objectives[ObjectiveNumber].NumberRequired -= UpdateValue;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, FString::Printf(TEXT("You need to do it %d more times"),
		Objectives[ObjectiveNumber].NumberRequired));

	if(Objectives[ObjectiveNumber].NumberRequired <= 0)
	{
		FinishObjective(ObjectiveNumber);
	}
}

void UQuestBase::FinishObjective(int ObjectiveNumber)
{
	if(ObjectiveNumber > Objectives.Num()) { return; }

	Objectives[ObjectiveNumber].bIsComplete = true;

	int NumberFinished = 0;
	for(int i = 0; i < Objectives.Num(); i++)
	{
		if(Objectives[i].bIsComplete)
		{
			NumberFinished++;
		}
	}

	if(NumberFinished >= Objectives.Num())
	{
		FinishQuest();
	}
}

void UQuestBase::FinishQuest()
{
}
