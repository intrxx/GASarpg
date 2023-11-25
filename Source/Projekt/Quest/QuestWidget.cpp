// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"

FText UQuestWidget::SetQuestName()
{
	FString SaveSlotNameShow = FString(TEXT("PlayerSaveSlot"));

	return FText::FromString(SaveSlotNameShow);
}

FText UQuestWidget::SetQuestDescription()
{
	FString SaveSlotNameShow = FString(TEXT("PlayerSaveSlot"));

	return FText::FromString(SaveSlotNameShow);
}
