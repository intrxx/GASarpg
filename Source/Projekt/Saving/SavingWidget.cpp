// Fill out your copyright notice in the Description page of Project Settings.


#include "SavingWidget.h"

#include "Projekt/Miscellaneous/MyGameInstance.h"



void USavingWidget::SavePressed()
{
	if(SelectedSlot)
	{
		Cast<UMyGameInstance>(GetGameInstance())->SaveGame(SelectedSlot);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("No selected slot on saving widget c++"));
	}
}
