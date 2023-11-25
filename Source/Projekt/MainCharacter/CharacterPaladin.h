// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "CharacterPaladin.generated.h"


/**
 * 
 */
UCLASS()
class PROJEKT_API ACharacterPaladin : public AMainCharacter
{
	GENERATED_BODY()

public:
	ACharacterPaladin(const FObjectInitializer& ObjectInitializer);

	//Player HUD
	class UUPlayerResourceWidget* GetFloatingResourceWidget();
	
	virtual void Tick(float DeltaTime) override;
	
	//GAS Server function
	virtual void PossessedBy(AController* NewController) override;

	virtual void FinishDying() override;

protected:
	virtual void BeginPlay() override;

	//Player HUD
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UUPlayerResourceWidget> FloatingResourceWidgetClass;

	UPROPERTY()
	class UUPlayerResourceWidget* FloatingResourceWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UWidgetComponent* FloatingResourceWidgetComponent;
	
	void BindASCInput();
	
	//GAS client function
	virtual void OnRep_PlayerState() override;

	//Initialize the bar, safe to call many times because it checks to make sure it only executes once
	UFUNCTION()
	void InitializeResourceStatusBar();
	
};
