// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "EnemyBase.generated.h"

class AEnemyBase;
class UWidgetComponent;

USTRUCT()
struct FEnemyData
{
	GENERATED_BODY()

	// Save Enemy
	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TSubclassOf<AEnemyBase> EnemyClass;
};

UCLASS()
class PROJEKT_API AEnemyBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void CheckQuestOnDeath();

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	//GAS Implementation
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void RemoveCharacterAbilities();
	
	UPROPERTY(BlueprintReadOnly)
	FTransform OriginTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	UWidgetComponent* HealthWidget;
	
	// Doesnt do anything yet
	UPROPERTY()
	FEnemyData EnemyData;

	//Handling Death
	void Die();

	UFUNCTION(BlueprintCallable, Category="EnemyBase|DeathHandling")
	void FinishDying();

	UFUNCTION(BlueprintCallable, Category="EnemyBase|DeathHandling")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category= "EnemyBase|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category= "EnemyBase|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category= "EnemyBase|Attributes")
	float GetMoveSpeedBaseValue() const;
	
	//Timer for clearing dead enemy
	FTimerHandle DelayEnemyRemovalTimer;
	UPROPERTY(EditAnywhere, Category="EnemyBase|DeathHandling")
	float DelayEnemyRemoval = 5.0f;

	UPROPERTY()
	bool bIsDying = false;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Ability System Component 
	TWeakObjectPtr<class UGASAbilitySystemComponent> EnemyAbilitySystemComponent;
	TWeakObjectPtr<class UGASAttributeSet> EnemyAttributeSetBase;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS");
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS");
	TArray <TSubclassOf <class UGASGameplayAbility>> DefaultAbilities;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();
	
	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	
	void RotateWidgetToPlayer();

	//Dying Tags and Montage
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EnemyBase|DeathHandling")
	UAnimMontage* DeathMontage;
	
	FGameplayTag EffectRemoveOnDeathTag;
	FGameplayTag DeadTag;
};
