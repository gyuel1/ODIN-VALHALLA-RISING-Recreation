// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace AttributeDataTags
{
	//Level
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_Level);
	// Health
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_Health);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_MaxHealth);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_HealthRegeneration);

	// Mana
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_Mana);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_MaxMana);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_ManaRegeneration);

	// Movement
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_MovementSpeed);

	// Offense
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_AttackPower);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_AttackSpeed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_Accuracy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_CriticalChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_CriticalDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_DamageIncrease);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_CrowdControlAccuracy);

	// Defense
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_Defense);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_Evasion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_DamageReduction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_CrowdControlResistance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_BaseStats_CrowdControlRecovery);

	//Transaction
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_Transaction_Gold);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_Transaction_CurrentExp);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_Transaction_MaxExp);

	//Reward OnlyTag
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Attribute_Transaction_Exp);
	
	
	//Skill
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Magnitude_DamagePercent);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Magnitude_AttackCount);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Magnitude_Skill_Cost);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Magnitude_Skill_Cooldown);

	//Weight (가중치)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Magnitude_Reward_Weight);
	
	
}

namespace CrowdControlTags
{
	
}

namespace StateTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_State_Dead);
}

namespace TriggerTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Init_FillVitals);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Init_Regeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Init_MaxExp);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Update_LevelUp);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Transaction_FieldMobReward);
}

namespace GameplayAbilityTags
{
	//Block
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Block_Combat);

	//Init
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Init_Stats);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Init_FillVitals);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Init_Regeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Init_MaxExp);

	//State
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Combat_State_Death);
	
	//Update
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Update_LevelUp);
	
	//Attack
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Combat_NormalAttack);

	//Transaction
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_Transaction_FieldMobReward);
}


namespace GameplayEffectTags
{
	//Init
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Init_FillVitals);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Init_Regeneration);
	
	//Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Combat_OnDamage);

	//Cost
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Skill_Cost);

	//Transaction
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Transaction_FieldReward);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Transaction_Gold);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Transaction_Exp);	//리워드 전용 태그

	//BossMonster
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Effect_Enrage_Status);
}

namespace GameplayAbilitySkillTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_TestSkill);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_TestSkill_Cooldown);

	//Valkyrie
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_001_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_002);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_002_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_003);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_003_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_004);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_004_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_005);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_005_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_006);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_006_Cooldown);

	//DarkMagician
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_001_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_002);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_002_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_003);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_003_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_004);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_004_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_005);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_005_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_006);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Valkyrie_006_Cooldown);

	// DarkMagician
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_001_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_002);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_002_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_003);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_003_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_004);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_004_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_005);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_005_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_006);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_006_Cooldown);

	// DarkMagician
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_001_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_002);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_002_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_003);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_003_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_004);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_004_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_005);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_005_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_006);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_DarkMagician_006_Cooldown);

	// Sniper
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_001_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_002);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_002_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_003);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_003_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_004);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_004_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_005);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_005_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_006);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Active_Sniper_006_Cooldown);
}

namespace NotifyTags
{
	//Hit
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Hit_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Hit_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Hit_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Hit_4);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Hit_5);

	//Spawn
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Spawn_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Spawn_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Spawn_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Spawn_4);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Combat_Spawn_5);

	//Trigger
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Trigger_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Trigger_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Trigger_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Trigger_4);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_Trigger_5);

	//ANS
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_ANS_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_ANS_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_ANS_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_ANS_4);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Notify_ANS_5);
}

namespace MonsterSkillAttackType
{
	//Monster Skill
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_AttackType_Skill)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(OD_Ability_AttackType_Skill_AOE_SingleHit)
	
}
