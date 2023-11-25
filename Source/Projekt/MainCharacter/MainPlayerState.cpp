// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"

#include "CharacterPaladin.h"
#include "Projekt/GAS/GASAbilitySystemComponent.h"
#include "MainCharacter.h"
#include "Projekt/Miscellaneous/EIGameplayTags.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "MainCharacterHUDWidget.h"
#include "MainCharacterPlayerController.h"
#include "UPlayerResourceWidget.h"

AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSetBase = CreateDefaultSubobject<UGASAttributeSet>(TEXT("AttributeSetBase"));
	
	NetUpdateFrequency = 100.0f;
	
	const FEIGameplayTags& GameplayTags = FEIGameplayTags::Get();
	DeadTag = GameplayTags.State_Dead;
}

UAbilitySystemComponent * AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGASAttributeSet * AMainPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool AMainPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AMainPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AMainPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AMainPlayerState::GetResource() const
{
	return AttributeSetBase->GetResource();
}

float AMainPlayerState::GetMaxResource() const
{
	return AttributeSetBase->GetMaxResource();
}

float AMainPlayerState::GetHealthRegen() const
{
	return AttributeSetBase->GetHealthRegen();
}

float AMainPlayerState::GetResourceRegen() const
{
	return AttributeSetBase->GetResourceRegen();
}

int32 AMainPlayerState::GetExperience() const
{
	return AttributeSetBase->GetExperience();
}

int32 AMainPlayerState::GetMaxExperience() const
{
	return AttributeSetBase->GetMaxExperience();
}

int32 AMainPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

float AMainPlayerState::GetMoveSpeed()
{
	return AttributeSetBase->GetMoveSpeed();
}


void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
		if(AbilitySystemComponent)
		{
			HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetHealthAttribute()).AddUObject(this,&AMainPlayerState::HealthChanged);
			MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AMainPlayerState::MaxHealthChanged);
			ResourceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetResourceAttribute()).AddUObject(this, &AMainPlayerState::ResourceChanged);
			MaxResourceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetMaxResourceAttribute()).AddUObject(this, &AMainPlayerState::MaxResourceChanged);
			HealthRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetHealthRegenAttribute()).AddUObject(this, &AMainPlayerState::HealthRegenChanged);
			ResourceRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetResourceRegenAttribute()).AddUObject(this, &AMainPlayerState::ResourceRegenChanged);
			ExperienceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetExperienceAttribute()).AddUObject(this, &AMainPlayerState::ExperienceChanged);
			CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetCharacterLevelAttribute()).AddUObject(this, &AMainPlayerState::CharacterLevelChanged);
			MaxExperienceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				AttributeSetBase->GetMaxExperienceAttribute()).AddUObject(this, &AMainPlayerState::MaxExperienceChanged);
			

			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("EffectTag.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);
		}
	
}

void AMainPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	
	ACharacterPaladin* Player = Cast<ACharacterPaladin>(GetPawn());
	
	if(Player)
	{
		UUPlayerResourceWidget* PlayerResourceWidget = Player->GetFloatingResourceWidget();
		if(PlayerResourceWidget)
		{
			PlayerResourceWidget->SetHeathPercentage(Health / GetMaxHealth());
		}	
	}
	
	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetPlayerController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->SetCurrentHealth(Health);
			HUD->SetHealthPercentage(Health / GetMaxHealth());
		}
	}
	
	if(!IsAlive())
	{
		if(Player)
		{
			Player->Die();
		}
	}
	
}

void AMainPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;
	
	ACharacterPaladin* Player = Cast<ACharacterPaladin>(GetPawn());
	if(Player)
	{
		UUPlayerResourceWidget* PlayerResourceWidget = Player->GetFloatingResourceWidget();
		if(PlayerResourceWidget)
		{
			PlayerResourceWidget->SetHeathPercentage(GetHealth() / MaxHealth);
		}
	}

	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetPlayerController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->SetMaxHealth(MaxHealth);
			HUD->SetHealthPercentage(GetHealth() / MaxHealth);
		}
	}
}

void AMainPlayerState::ResourceChanged(const FOnAttributeChangeData& Data)
{
	float Resource = Data.NewValue;

	ACharacterPaladin* Player = Cast<ACharacterPaladin>(GetPawn());
	if(Player)
	{
		UUPlayerResourceWidget* PlayerResourceWidget = Player->GetFloatingResourceWidget();
		if(PlayerResourceWidget)
		{
			PlayerResourceWidget->SetResourcePercentage(Resource / GetMaxResource());
		}
	}

	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetPlayerController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->SetCurrentResource(Resource);
			HUD->SetResourcePercentage(Resource / GetMaxResource());
		}
	}
}

void AMainPlayerState::MaxResourceChanged(const FOnAttributeChangeData& Data)
{
	float MaxResource = Data.NewValue;

	
	ACharacterPaladin* Player = Cast<ACharacterPaladin>(GetPawn());
	if(Player)
	{
		UUPlayerResourceWidget* PlayerResourceWidget = Player->GetFloatingResourceWidget();
		if(PlayerResourceWidget)
		{
			PlayerResourceWidget->SetResourcePercentage(GetResource() / MaxResource);
		}
	}

	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetPlayerController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->SetMaxHealth(MaxResource);
			HUD->SetResourcePercentage(GetResource() / MaxResource);
		}
	}
}

void AMainPlayerState::HealthRegenChanged(const FOnAttributeChangeData& Data)
{
	float HealthRegen = Data.NewValue;

	//TODO Planing on creating special Character tab with all the stats
}

void AMainPlayerState::ResourceRegenChanged(const FOnAttributeChangeData& Data)
{
	float ResourceRegen = Data.NewValue;
	
	//TODO Planing on creating special Character tab with all the stats
}

void AMainPlayerState::ExperienceChanged(const FOnAttributeChangeData& Data)
{
	float Experience = Data.NewValue;

	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetPlayerController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->SetCurrentExperience(Experience);
			HUD->SetExperiencePercentage(Experience / GetMaxExperience());
		}
	}
}

void AMainPlayerState::MaxExperienceChanged(const FOnAttributeChangeData& Data)
{
	float MaxExperience = Data.NewValue;

	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetPlayerController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->SetMaxExperience(MaxExperience);
			HUD->SetExperiencePercentage(GetExperience() / MaxExperience);
		}
	}
}


void AMainPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	float CharacterLevel = Data.NewValue;

	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetPlayerController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			HUD->SetCharacterLevel(CharacterLevel);
		}
	}
}


void AMainPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if(NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Hero")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun.Hero")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}












