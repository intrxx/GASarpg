// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnButton.h"

void URespawnButton::Init()
{
	RButton->OnClicked.AddUniqueDynamic(this, &URespawnButton::OnButtonClicked);
}

void URespawnButton::AddChildText(UTextBlock* Text)
{
	TextBlock_Respawn = Text;
}

void URespawnButton::OnButtonClicked()
{
	OnButtonClickedDelegate.Broadcast(this);
}


