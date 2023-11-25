// Fill out your copyright notice in the Description page of Project Settings.


#include "EIGameplayTags.h"
#include "GameplayTagsManager.h"



FEIGameplayTags FEIGameplayTags::GameplayTags;

void FEIGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FEIGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	// Movement/Utility input tags
	AddTag(InputTag_Move, "InputTag.Move", "MoveForward input.");
	AddTag(InputTag_RotatePlayer, "InputTag.RotatePlayer", "RotatePlayer input.");
	AddTag(InputTag_ZoomCameraIn, "InputTag.ZoomCameraIn", "ZoomCameraIn input.");
	AddTag(InputTag_ZoomCameraOut, "InputTag.ZoomCameraOut", "ZoomCameraOut input.");
	AddTag(InputTag_OpenInventory, "InputTag.OpenInventory", "OpenInventory input.");
	AddTag(InputTag_Pickup, "InputTag.Pickup", "Pickup input.");
	AddTag(InputTag_MenuSwitch, "InputTag.MenuSwitch", "Switch between opening/closing menu.");
	AddTag(InputTag_QuestSwitch, "InputTag.QuestSwitch", "Switch between opening/closing quests.");
	AddTag(InputTag_ToggleFloatingWidget, "InputTag.ToggleFloatingWidget", "ToggleFloatingWidget input.");
	AddTag(InputTag_ToggleSpellTree, "InputTag.ToggleSpellTree", "ToggleSpellTree input.");

	// Event Tags
	AddTag(HitTag_LightAttack,"HitTag.LightAttack","LightAttackHit tag.");
	AddTag(HitTag_HeavyAttack, "HitTag.HeavyAttack","HeavyAttackHit tag.");
	AddTag(Block_Attacks, "BlockTag.Attack", "BlockAttack tag.");

	// Abilities hero
	AddTag(InputTag_FirstAbility, "InputTag.Ability.First", "FirstAbility input.");
	AddTag(InputTag_SecondAbility, "InputTag.Ability.Second", "SecondAbility input.");
	AddTag(InputTag_ThirdAbility, "InputTag.Ability.Third", "ThirdAbility input.");
	AddTag(InputTag_ForthAbility, "InputTag.Ability.Fourth", "FourthAbility input.");
	AddTag(InputTag_LightAttack, "InputTag.LightAttack", "LightAttack input.");
	AddTag(InputTag_HeavyAttack, "InputTag.HeavyAttack", "HeavyAttack input.");
	AddTag(InputTag_TimestopAbility, "InputTag.Ability.Timestop", "TimestopAbility input.");
	AddTag(InputTag_TimestopCancelLastMove, "InputTag.Ability.TimestopCancelLastMove", "Timestop cancel last move input.");
	AddTag(InputTag_PotionUse, "InputTag.Ability.HealingPotion", "Using potion input.");

	// Abilities enemies
	AddTag(HorseheadAttack, "InputTag.Ability.Horsehead.Attack", "Horsehead base attack input");
	AddTag(LimiterAttack, "InputTag.Ability.Limiter.Attack", "Limiter base attack input");
	AddTag(LimiterThrow, "InputTag.Ability.Limiter.Throw", "Limiter throw input");
	AddTag(SheeterAttack, "InputTag.Ability.Sheeter.Attack", "Sheeter base attack input");
	AddTag(SheeterPoison, "InputTag.Ability.Sheeter.Poison", "Sheeter poison attack input");
	
	// Events enemies
	AddTag(HitTag_HorseheadAttack, "HitTag.HorseheadAttack","Horsehead attack tag.");
	AddTag(HitTag_LimiterAttack, "HitTag.LimiterAttack","HitTag.LimiterAttack");
	AddTag(HitTag_LimiterAttack, "HitTag.LimiterThrow","Limiter throw tag.");
	AddTag(HitTag_SheeterAttack, "HitTag.SheeterAttack","Sheeter attack tag.");
	
	// Cooldowns
	AddTag(Cooldown_Dash, "Cooldown.Ability.Dash", "DashAbility cooldown.");
	AddTag(Cooldown_Shockwave, "Cooldown.Ability.Shockwave", "ShockwaveAbility cooldown.");
	AddTag(Cooldown_Puncture, "Cooldown.Ability.Puncture", "PunctureAbility cooldown.");
	AddTag(Cooldown_Guard, "Cooldown.Ability.Guard", "Tag used to inform Guard ability that it is on cooldown.");
	AddTag(Cooldown_GreatHammer, "Cooldown.Ability.GreatHammer", "Tag used to inform GreatHammer ability that it is on cooldown.");
	AddTag(Cooldown_HealingPotion, "Cooldown.Ability.HealingPotion", "Tag used to inform HealingPotionAbility that it is on cooldown.");
	
	// States and Effects
	AddTag(State_Dead, "StateTag.Dead","Dead state.");
	AddTag(Effect_RemoveOnDeath, "EffectTag.RemoveOnDeath", "RemoveOnDeath effect.");
	AddTag(Timestop, "EffectTag.Timestop", "Timestop effect.");
	AddTag(Effect_Guard, "EffectTag.Guard", "Guard attacks.");
	AddTag(Debuff_Stun, "EffectTag.Debuff.Stun", "Tag used to apply to stunned actor");
	AddTag(Debuff_Oiled, "EffectTag.Debuff.Oiled", "Tag used to apply to oiled actor");
	AddTag(Debuff_Burning, "EffectTag.Debuff.Burning", "Tag used to apply to burning actor");
	AddTag(Owning_HealingPotion, "EffectTag.Owning.HealingPotion", "Tag used to notify PotionHealingAbility that Character owns a potion");

	// Abilities owned tags
	// Enemy
	AddTag(Horsehead_BaseAttack, "Ability.Enemy.HorseheadBaseAttack", "Tag used to notify ASC that HorseheadBaseAttack is active");
	AddTag(Limiter_BaseAttack, "Ability.Enemy.LimiterBaseAttack", "Tag used to notify ASC that LimiterBaseAttack is active");
	AddTag(Sheeter_BaseAttack, "Ability.Enemy.SheeterBaseAttack", "Tag used to notify ASC that SheeterBaseAttack is active");
	AddTag(Sheeter_Poison, "Ability.Enemy.SheeterPoison", "Tag used to notify ASC that SheeterPoison is active");
	AddTag(Limiter_Throw, "Ability.Enemy.LimiterThrow", "Tag used to notify ASC that LimiterThrow is active");
	
	// Hero
	AddTag(Hero_Dash, "Ability.Hero.Dash", "Tag used to notify ASC that Dash is active");
	AddTag(Hero_Guard, "Ability.NotCanceledByStun.Hero.Guard", "Tag used to notify ASC that Guard is active");
	AddTag(Hero_Puncture, "Ability.Hero.Puncture", "Tag used to notify ASC that Puncture is active");
	AddTag(Hero_Shockwave, "Ability.Hero.Shockwave", "Tag used to notify ASC that Shockwave is active");
	AddTag(Hero_GreatHammer, "Ability.Hero.GreatHammer", "Tag used to notify ASC that GreatHammer is active");
	AddTag(Hero_LBaseAttack, "Ability.Hero.LBaseAttack", "Tag used to notify ASC that LBaseAttack is active");
	
}

void FEIGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}


