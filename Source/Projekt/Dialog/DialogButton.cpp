// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogButton.h"

void UDialogButton::Init()
{
	MainButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
}

void UDialogButton::AddChildText(UTextBlock* Text)
{
	TextBlock = Text;
}

void UDialogButton::OnButtonClicked()
{
	// Grabs reference to clicked button
	OnClickedDelegate.Broadcast(this);
}
