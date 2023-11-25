// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class PROJEKT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void ContinuePressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonContinue;
	
	UFUNCTION(BlueprintCallable)
	void NewGamePressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonNewGame;

	UFUNCTION(BlueprintCallable)
	void LoadGamePressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonLoadGame;

	UFUNCTION(BlueprintCallable)
	void SaveGamePressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonSaveGame;

	UFUNCTION(BlueprintCallable)
	void OptionsPressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonOptions;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonCredits;
	
	UFUNCTION(BlueprintCallable)
	void QuitPressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonQuit;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> SavingWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> OptionsWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* OptionsWidget;
};
