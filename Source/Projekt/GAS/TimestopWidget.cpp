// Fill out your copyright notice in the Description page of Project Settings.


#include "TimestopWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UTimestopWidget::AddNewMoveToWidget(FString MoveName)
{
	UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	TextBlock->SetText(FText::FromString(MoveName));
	VerticalBox->AddChild(TextBlock);
}
