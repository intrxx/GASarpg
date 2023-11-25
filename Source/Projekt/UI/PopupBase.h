// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupBase.generated.h"

class USizeBox;
class UButton;
class UOverlay;
class UImage;
class UTextBlock;
class UVerticalBox;


UCLASS()
class PROJEKT_API UPopupBase : public UUserWidget
{
	GENERATED_BODY()

public:

	// Widget params main
	// Offset from left side of the screen with some starting padding
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	float OffsetFromLeft = 50;
	// Offset from top side of the screen with some starting padding
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	float OffsetFromTop = 50;
	// Overall widget width
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	float WidgetSizeX = 400;
	// Overall widget height
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	float WidgetSizeY = 200;

	/*
	// In 0-1 preferably top/mid/bot should sum to 1 to keep the ratio
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	float TopPartSize;
	// In 0-1 preferably top/mid/bot should sum to 1 to keep the ratio
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	float MidPartSize;
	// In 0-1 preferably top/mid/bot should sum to 1 to keep the ratio
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	float BotPartSize;*/

	// Top text should probably be a short summarize what popup is about
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	FText TopText;
	// Mid text should probably be detailed explanation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	FText MidText;
	// Bot text should probably be just default closing info
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Parameters")
	FText BotText;

	// Widget components refs
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* SizeBoxRoot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonRoot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayRoot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayContentTop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayContentMid;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayContentBot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageBackgroundTop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageBackgroundBot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextTop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextMid;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBot;
	
}; 
