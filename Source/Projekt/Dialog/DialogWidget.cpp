// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogWidget.h"

#include "Dialog.h"
#include "DialogButton.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

// Updates UI with new subtitles and adds widget to viewport if not already seen
void UDialogWidget::UpdateSubtitles(FDialogM Subtitles)
{
	DialogBox->SetText(FText::FromString(Subtitles.NPCSubtitle));

	// Cleanup of previous buttons
	for (auto Button : ButtonArray)
	{
		Button->RemoveFromParent();
	}

	ButtonArray.Empty();
	
	for (auto Question : Subtitles.Subtitles)
	{
		// Creates Button/Text for give question
		UDialogButton* Button = WidgetTree->ConstructWidget<UDialogButton>(DialogButtonClass);
		
		Button->TextBlock->SetText(FText::FromString(Question.Subtitle));
		Button->Init();
		Button->OnClickedDelegate.AddDynamic(this, &UDialogWidget::PlayGivenLines);

		// Adds Button to the array for later cleanup
		ButtonArray.Add(Button);

		// Adds button to array to later destroy the button on next question
		VerticalBox->AddChild(Button);
	}

	if(!IsInViewport())
	{
		AddToViewport();
	}
}

// Decouples Dialog with NPC and tries to update UI with new lines
void UDialogWidget::PlayGivenLines(UDialogButton* Button)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UDialog* DialogComponent = MainCharacter->GetDialogComponent();
	DialogComponent->Talk(Button->TextBlock->GetText().ToString());
}
