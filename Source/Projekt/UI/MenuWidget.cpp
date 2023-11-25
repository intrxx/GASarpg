// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

#include "Projekt/Saving/LoadingWidget.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Projekt/Saving/SaveClass.h"
#include "Projekt/Saving/SaveSlotWidget.h"
#include "Projekt/Saving/SavingWidget.h"
#include "Projekt/AI/BTTask_ActivateAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(OptionsWidgetClass)
	{
		OptionsWidget = CreateWidget(GetWorld(), OptionsWidgetClass);
	}
}

void UMenuWidget::ContinuePressed()
{
	if(UGameplayStatics::GetCurrentLevelName(GetWorld()) == FString(TEXT("MenuLevel")))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
	}
	else
	{
		RemoveFromViewport();
		GetWorld()->GetFirstPlayerController()->SetPause(false);

		Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())->bIsMenuOpen = false;
	}
}

void UMenuWidget::NewGamePressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Default")));
}

void UMenuWidget::LoadGamePressed()
{
	if(LoadingWidgetClass)
	{
		ULoadingWidget* LoadWidget = Cast<ULoadingWidget>(CreateWidget(GetWorld(), LoadingWidgetClass));

		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot1);
		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot2);
		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot3);
		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot4);
		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot5);
		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot6);
		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot7);
		LoadWidget->SaveSlotArray.Add(LoadWidget->SaveSlot8);
		for (auto SaveSlot : LoadWidget->SaveSlotArray)
		{
			SaveSlot->OwningWidget = LoadWidget;
			
			if(USaveClass* LoadedGame = Cast<USaveClass>(UGameplayStatics::LoadGameFromSlot(SaveSlot->SaveSlotName, 0)))
			{
				SaveSlot->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
				SaveSlot->SetVisibility(ESlateVisibility::Visible);
				LoadedGame = nullptr;
			}
			else
			{
				SaveSlot->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.2f));
				SaveSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		LoadWidget->AddToViewport();
	}
}

void UMenuWidget::SaveGamePressed()
{
	if(SavingWidgetClass)
	{
		USavingWidget* SaveWidget = Cast<USavingWidget>(CreateWidget(GetWorld(), SavingWidgetClass));
		SaveWidget->SaveSlot1->OwningWidget = SaveWidget;
		SaveWidget->SaveSlot2->OwningWidget = SaveWidget;
		SaveWidget->SaveSlot3->OwningWidget = SaveWidget;
		SaveWidget->SaveSlot4->OwningWidget = SaveWidget;
		SaveWidget->SaveSlot5->OwningWidget = SaveWidget;
		SaveWidget->SaveSlot6->OwningWidget = SaveWidget;
		SaveWidget->SaveSlot7->OwningWidget = SaveWidget;
		SaveWidget->SaveSlot8->OwningWidget = SaveWidget;
		SaveWidget->AddToViewport();
	}
}

void UMenuWidget::OptionsPressed()
{
	if(OptionsWidget)
	{
		OptionsWidget->AddToViewport();
	}
}

void UMenuWidget::QuitPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
