// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialog.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UDialogButton;
struct FDialogM;
/**
 * 
 */

UCLASS()
class PROJEKT_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Bind subs
	UPROPERTY(BlueprintReadOnly)
	FString SubtitleToDisplay;
 
	// Updates the displayed subtitles based on the given array
	UFUNCTION(BlueprintCallable, Category = DialogSystem)
	void UpdateSubtitles(FDialogM Subtitles);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogButton> DialogButtonClass;
	
	// Array of buttons that we are going to create/destroy based on questions provided
	TArray<UDialogButton*> ButtonArray;
 
	// This array will populate our buttons from within the show function
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> Questions;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> NewQuestionsArray;

	UFUNCTION(BlueprintCallable)
	void PlayGivenLines(UDialogButton* Button);

	// References to BP 
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DialogBox;
};
