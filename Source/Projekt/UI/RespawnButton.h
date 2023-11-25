// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RespawnButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonClickedDelegate, class URespawnButton*, Button);
/**
 * 
 */
UCLASS()
class PROJEKT_API URespawnButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Init();

	UFUNCTION()
	void AddChildText(UTextBlock* Text);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Respawn;

	FButtonClickedDelegate OnButtonClickedDelegate;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* RButton;

	UFUNCTION()
	void OnButtonClicked();
};
