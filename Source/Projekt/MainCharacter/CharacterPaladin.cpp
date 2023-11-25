// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPaladin.h"
#include "AbilitySystemComponent.h"
#include "Projekt/GAS/GASAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Projekt/GAS/GASGameplayAbility.h"
#include "MainCharacterPlayerController.h"
#include "UPlayerResourceWidget.h"
#include "MainPlayerState.h"
#include "Projekt/Miscellaneous/ProjektGameMode.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


ACharacterPaladin::ACharacterPaladin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FloatingResourceWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("FloatingStatusBarComponent"));
	FloatingResourceWidgetComponent->SetupAttachment(RootComponent);
	FloatingResourceWidgetComponent->SetRelativeLocation(FVector(0,0,120));
	FloatingResourceWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingResourceWidgetComponent->SetDrawSize(FVector2D(500,500));

	
	
	FloatingResourceWidgetClass = StaticLoadClass(UObject::StaticClass(),nullptr,TEXT("/Game/Blueprints/UI/PlayerWidgets/WBP_PlayerResourceWidget.WBP_PlayerResourceWidget_C"));
	if (!FloatingResourceWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find DamageNumberClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}
}

void ACharacterPaladin::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeResourceStatusBar();
}

void ACharacterPaladin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Health: %f"),GetHeath()));
}

void ACharacterPaladin::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if(PS)
	{
		//Setting the ASC on the server
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		AttributeSet = PS->GetAttributeSetBase();
		
		InitializeAttributes();

		//TODO Change later
		SetHealth(GetMaxHeath());
		SetResource(GetMaxResource());
		
		GiveStartupEffects();
		GiveAbilities();

		AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetController());
		if(PC)
		{
			PC->CreateHUD();
		}
		
		InitializeResourceStatusBar();
	}
	
}

void ACharacterPaladin::FinishDying()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		AProjektGameMode* GameMode = Cast<AProjektGameMode>(GetWorld()->GetAuthGameMode());

		if(GameMode)
		{
			GameMode->MainCharacterDied(GetController());
		}
	}
	
	Super::FinishDying();
}


void ACharacterPaladin::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if(PS)
	{
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		BindASCInput();

		AttributeSet = PS->GetAttributeSetBase();

		InitializeAttributes();
			
		AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetController());
		if(PC)
		{
			PC->CreateHUD();
		}
		
		InitializeResourceStatusBar();
		
		SetHealth(GetMaxHeath());
		SetResource(GetMaxResource());
	}
}

void ACharacterPaladin::BindASCInput()
{
	if (AbilitySystemComponent.IsValid() && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

UUPlayerResourceWidget* ACharacterPaladin::GetFloatingResourceWidget()
{
	return FloatingResourceWidget;
}

void ACharacterPaladin::InitializeResourceStatusBar()
{
	//Only creates one
	if (FloatingResourceWidget || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if(PC)
	{
		if(FloatingResourceWidgetClass)
		{
			FloatingResourceWidget = CreateWidget<UUPlayerResourceWidget>(PC, FloatingResourceWidgetClass);
			if(FloatingResourceWidget && FloatingResourceWidgetClass)
			{
				FloatingResourceWidgetComponent->SetWidget(FloatingResourceWidget);

				FloatingResourceWidget->SetHeathPercentage(GetHeath() / GetMaxHeath());
				FloatingResourceWidget->SetResourcePercentage(GetResource() / GetMaxResource());
			}
		}
	}
}





