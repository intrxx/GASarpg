// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "DialogButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBUIOnClickedSignature, class UDialogButton*, Button);

class UTextBlock;

UCLASS()
class PROJEKT_API UDialogButton : public UUserWidget
{
	GENERATED_BODY()

public:
	// Bind a function with the signature "void OnClicked(UBUIUWButton* Button);
	FBUIOnClickedSignature OnClickedDelegate;

	UFUNCTION()
	void Init();

	UFUNCTION()
	void AddChildText(UTextBlock* Text);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* MainButton;
	
	UFUNCTION()
	void OnButtonClicked();
};
