// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "Projekt/Input/EIInputConfig.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MainCharacter.generated.h"

enum class EGASAbilityInputID : uint8;
class UMainCharacterInventory;
class UItemObject;
class UDialog;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AMainCharacter*, Character);

UCLASS()
class PROJEKT_API AMainCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter(const FObjectInitializer& ObjectInitializer);

	//UPROPERTY(BlueprintAssignable, Category = "BaseCharacter")
	//FCharacterDiedDelegate OnCharacterDied;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//----GAMEPLAY ABILITY SYSTEM----
	// Implementation of IAblitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//Getters for attributes from UGASAttributeSet
	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetHeath() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxHeath() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetResource() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxResource() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetHealthRegen() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetResourceRegen() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	int32 GetExperience() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMoveSpeedBaseValue() const;
	
	/*
	 * 
	 */

	
	UFUNCTION(BlueprintCallable)
	void UseItem(UItemObject* Item);

	virtual void RemoveCharacterAbilities();
	
	virtual void Die();

	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void FinishDying();
	
	// Getters
	UFUNCTION()
	UMainCharacterInventory* GetInventoryComponent();

	UFUNCTION()
	UDialog* GetDialogComponent();

	class UCameraComponent* GetCamera() const;
	class USpringArmComponent* GetSpringArm() const;
	
	// Event tags and flags
	FGameplayTag BlockAbilityUse;
	FGameplayTag MenuOpened;

	UPROPERTY()
	bool bIsMenuOpen = false;
	
	//Player HUD
	UPROPERTY(BlueprintReadWrite)
	class UQuestBase* CurrentQuest;

	UPROPERTY(BlueprintReadWrite)
	class ATickTimestopActor* TimestopActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//----GAMEPLAY ABILITY SYSTEM----
	TWeakObjectPtr<class UGASAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UGASAttributeSet> AttributeSet;

	//Characters default abilities
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGASGameplayAbility>> DefaultAbilities;

	//Character default attributes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// Effects that are applied once on startup
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	//Initialize functions for Attributes and Abilities
	virtual void InitializeAttributes();
	virtual void GiveAbilities();
	virtual void GiveStartupEffects();
	
	// Setters for attributes *remember to only use it for respawn*
	virtual void SetHealth(float Health);
	virtual void SetResource(float Resource);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAIPerceptionStimuliSourceComponent* AIStimuliComponent;
	
	// Inventory properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "UI")
	UMainCharacterInventory* MainCharacterInventoryComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
	uint8 NumberOfInventorySlots = 140;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WidgetInventoryClass;
	
	UPROPERTY()
	UUserWidget* WidgetInventory;

	//Open inventory
	void OnMouseClick(const FInputActionValue& InputActionValue);

	UPROPERTY()
	bool bIsInventoryOpen = false;
	UPROPERTY()
	bool bIsQuestOpen = false;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestClass;
	
	UPROPERTY()
	UUserWidget* MenuWidget;
	UPROPERTY()
	UUserWidget* QuestWidget;
	
	UPROPERTY()
	UDialog* DialogComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "BaseCharacter")
	UAnimMontage* DeathMontage;
	
	FGameplayTag DeadTag;

private:
	//Movement components
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UEIInputConfig* InputConfig;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Rotate();
	void Input_ZoomCameraIn();
	void Input_ZoomCameraOut();
	void Input_ToggleFloatingWidget();
	void Input_ToggleSpellTree();

	//Ability input bindings
	void SendLocalInputToASC(bool bIsPressed, const EGASAbilityInputID AbilityInputID);
	void Input_LightAttack();
	void Input_HeavyAttack();
	void Input_FirstAbility();
	void Input_SecondAbility();
	void Input_ThirdAbility();
	void Input_FourthAbility();
	void Input_TimestopAbility();
	void Input_TimestopCancelLastMove();
	void Input_UsingHealingPotion();

	//Timer for delaying the deleting of attack block tag
	FTimerHandle DelayTagRemovalTimer;
	float DelayTagRemoval = 0.1;
	void RemoveTag();
	
	// Inventory functions
	void InventoryPressed();
	void MenuSwitch();
	void QuestSwitch();

	//Camera components
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	//Camera placement
	UPROPERTY(EditAnywhere, Category = "Camera Setting")
		float SpringArmLength = 800.f;
	UPROPERTY(EditAnywhere, Category = "Camera Setting")
		float CameraRotation = -50.f;
	
};
