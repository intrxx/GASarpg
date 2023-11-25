// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSlotWidget.h"

#include "LoadingWidget.h"
#include "Projekt/Miscellaneous/MyGameInstance.h"
#include "SavingWidget.h"
#include "Components/VerticalBox.h"

FText USaveSlotWidget::SlotName()
{
	FString SaveSlotNameShow = FString(TEXT("PlayerSaveSlot"));

	return FText::FromString(SaveSlotNameShow);
}

void USaveSlotWidget::ButtonPressed()
{
	if(bShouldSave)
	{
		UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		USavingWidget* SavingWidget = Cast<USavingWidget>(OwningWidget);
		
		if(SavingWidget && GI)
		{
			GI->LoadSaveInfo(this, SavingWidget);
			
			SavingWidget->SaveName = SaveSlotName;
			SavingWidget->VerticalBoxSaveInfo->SetVisibility(ESlateVisibility::Visible);
			SavingWidget->SelectedSlot = this;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("no loading or gi in saveslotwidget.cpp"));
		}

	}
	else if(bShouldLoad)
	{
		UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		ULoadingWidget* LoadingWidget = Cast<ULoadingWidget>(OwningWidget);

		if(LoadingWidget && GI)
		{
			GI->LoadSaveInfo(this, LoadingWidget);
			
			LoadingWidget->SaveName = SaveSlotName;
			LoadingWidget->VerticalBoxSaveInfo->SetVisibility(ESlateVisibility::Visible);
			LoadingWidget->SelectedSlot = this;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("no loading or gi in saveslotwidget.cpp"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("Save slot widget doesnt load/save in saveslotwidget.cpp"));
	}
}
