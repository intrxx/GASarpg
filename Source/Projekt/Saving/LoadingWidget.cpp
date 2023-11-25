// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingWidget.h"

#include "Projekt/Miscellaneous/MyGameInstance.h"

bool ULoadingWidget::Initialize()
{
	return Super::Initialize();
}

void ULoadingWidget::LoadPressed()
{
	if(SelectedSlot)
	{
		Cast<UMyGameInstance>(GetGameInstance())->LoadGame(SelectedSlot);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("No selected slot on saving widget c++"));
	}
}
