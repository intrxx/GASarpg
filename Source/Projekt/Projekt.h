// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

/* Stencil index mapping to PP_OutlineColored */
#define STENCIL_ITEMHIGHLIGHT 252;
#define STENCIL_NEUTRAL_OUTLINE 253;
#define STENCIL_ENEMY_OUTLINE 254;
#define STENCIL_FRIENDLY_OUTLINE 255;

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	LightAttack,
	HeavyAttack,
	FirstAbility,
	SecondAbility,
	ThirdAbility,
	ForthAbility,
	TimestopAbility,
	UseHealingPotion
};