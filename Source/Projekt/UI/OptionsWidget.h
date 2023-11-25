// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UButton;
class UComboBoxString;

UCLASS()
class PROJEKT_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual bool Initialize() override;

	UFUNCTION()
	void UpdateTooltips();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UUserWidget> ConfirmWidget;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UUserWidget> ResolutionWidget;

	UPROPERTY(BlueprintReadWrite)
	bool bIsResDirty = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsOtherDirty = false;
	
	UFUNCTION(BlueprintCallable)
	void BackPressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonBack;

	UFUNCTION(BlueprintCallable)
	void DefaultsPressed();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonDefaults;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> Defaults;

	UPROPERTY(BlueprintReadOnly)
	UGameUserSettings* UserSettings;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringWindow;
	UFUNCTION()
	void ComboBoxStringWindowUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringResolution;
	UFUNCTION()
	void ComboBoxStringResolutionUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringFPS;
	UFUNCTION()
	void ComboBoxStringFPSUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringVSync;
	UFUNCTION()
	void ComboBoxStringVSyncUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringTexture;
	UFUNCTION()
	void ComboBoxStringTextureUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringShadows;
	UFUNCTION()
	void ComboBoxStringShadowsUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringAA;
	UFUNCTION()
	void ComboBoxStringAAUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringPP;
	UFUNCTION()
	void ComboBoxStringPPUpdate(FString SelectedItem, ESelectInfo::Type SelectionType);
};
