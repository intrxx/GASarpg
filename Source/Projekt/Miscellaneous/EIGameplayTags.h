// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FEIGameplayTags
{
public:
	static const FEIGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();
	
	//Movement / Utility input tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_RotatePlayer;
	FGameplayTag InputTag_ZoomCameraIn;
	FGameplayTag InputTag_ZoomCameraOut;
	FGameplayTag InputTag_Pickup;
	FGameplayTag InputTag_OpenInventory;
	FGameplayTag InputTag_LightAttack;
	FGameplayTag InputTag_HeavyAttack;
	FGameplayTag InputTag_MenuSwitch;
	FGameplayTag InputTag_QuestSwitch;
	FGameplayTag InputTag_ToggleFloatingWidget;
	FGameplayTag InputTag_ToggleSpellTree;
	
	//Abilities hero
	FGameplayTag HitTag_LightAttack;
	FGameplayTag HitTag_HeavyAttack;
	FGameplayTag Block_Attacks;
	FGameplayTag InputTag_FirstAbility;
	FGameplayTag InputTag_SecondAbility;
	FGameplayTag InputTag_ThirdAbility;
	FGameplayTag InputTag_ForthAbility;
	FGameplayTag InputTag_TimestopAbility;
	FGameplayTag InputTag_TimestopCancelLastMove;
	FGameplayTag InputTag_PotionUse;
	
	// Abilities enemy
	FGameplayTag HorseheadAttack;
	FGameplayTag LimiterAttack;
	FGameplayTag LimiterThrow;
	FGameplayTag SheeterAttack;
	FGameplayTag SheeterPoison;

	// Abilities owned tags
	// Enemy
	FGameplayTag Horsehead_BaseAttack;
	FGameplayTag Limiter_BaseAttack;
	FGameplayTag Limiter_Throw;
	FGameplayTag Sheeter_BaseAttack;
	FGameplayTag Sheeter_Poison;

	// Hero
	FGameplayTag Hero_LBaseAttack;
	FGameplayTag Hero_Dash;
	FGameplayTag Hero_Guard;
	FGameplayTag Hero_Shockwave;
	FGameplayTag Hero_Puncture;
	FGameplayTag Hero_GreatHammer;

	// Events enemy
	FGameplayTag HitTag_HorseheadAttack;
	FGameplayTag HitTag_LimiterAttack;
	FGameplayTag HitTag_LimiterThrow;
	FGameplayTag HitTag_SheeterAttack;
	
	//Cooldowns
	FGameplayTag Cooldown_Dash;
	FGameplayTag Cooldown_Shockwave;
	FGameplayTag Cooldown_Puncture;
	FGameplayTag Cooldown_Guard;
	FGameplayTag Cooldown_GreatHammer;
	FGameplayTag Cooldown_HealingPotion;
	
	//States and Effects
	FGameplayTag State_Dead;
	FGameplayTag Effect_RemoveOnDeath;
	FGameplayTag Effect_Guard;
	FGameplayTag Timestop;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Burning;
	FGameplayTag Debuff_Oiled;
	FGameplayTag Owning_HealingPotion;

protected:

	//Register all the tags with the manager
	void AddAllTags(UGameplayTagsManager& Manager);

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:

	static FEIGameplayTags GameplayTags;
};