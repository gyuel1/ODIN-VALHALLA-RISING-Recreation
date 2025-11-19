// Fill out your copyright notice in the Description page of Project Settings.


#include "Tags/ODAbilityTags.h"

namespace AttributeDataTags
{
	//Level
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_Level,  "OD.Attribute.BaseStats.Level");
	
	// Health
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_Health, "OD.Attribute.BaseStats.Health");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_MaxHealth, "OD.Attribute.BaseStats.MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_HealthRegeneration, "OD.Attribute.BaseStats.HealthRegeneration");

	// Mana
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_Mana, "OD.Attribute.BaseStats.Mana");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_MaxMana, "OD.Attribute.BaseStats.MaxMana");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_ManaRegeneration, "OD.Attribute.BaseStats.ManaRegeneration");

	// Movement
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_MovementSpeed, "OD.Attribute.BaseStats.MovementSpeed");

	// Offense
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_AttackPower, "OD.Attribute.BaseStats.AttackPower");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_AttackSpeed, "OD.Attribute.BaseStats.AttackSpeed");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_Accuracy, "OD.Attribute.BaseStats.Accuracy");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_CriticalChance, "OD.Attribute.BaseStats.CriticalChance");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_CriticalDamage, "OD.Attribute.BaseStats.CriticalDamage");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_DamageIncrease, "OD.Attribute.BaseStats.DamageIncrease");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_CrowdControlAccuracy, "OD.Attribute.BaseStats.CrowdControlAccuracy");

	// Defense
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_Defense, "OD.Attribute.BaseStats.Defense");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_Evasion, "OD.Attribute.BaseStats.Evasion");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_DamageReduction, "OD.Attribute.BaseStats.DamageReduction");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_CrowdControlResistance,
	                       "OD.Attribute.BaseStats.CrowdControlResistance");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_BaseStats_CrowdControlRecovery, "OD.Attribute.BaseStats.CrowdControlRecovery");

	//Transaction
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_Transaction_Gold,   "OD.Attribute.Transaction.Gold");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_Transaction_CurrentExp,    "OD.Attribute.Transaction.CurrentExp");
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_Transaction_MaxExp, "OD.Attribute.Transaction.MaxExp");

	//Reward OnlyTag
	UE_DEFINE_GAMEPLAY_TAG(OD_Attribute_Transaction_Exp, "OD.Attribute.Transaction.Exp");

	//Magnitude
	UE_DEFINE_GAMEPLAY_TAG(OD_Magnitude_DamagePercent, "OD.Magnitude.DamagePercent");
	UE_DEFINE_GAMEPLAY_TAG(OD_Magnitude_AttackCount, "OD.Magnitude.AttackCount");
	
	UE_DEFINE_GAMEPLAY_TAG(OD_Magnitude_Skill_Cost, "OD.Magnitude.Skill.Cost");
	UE_DEFINE_GAMEPLAY_TAG(OD_Magnitude_Skill_Cooldown, "OD.Magnitude.Skill.Cooldown");

	//Weight (가중치)
	UE_DEFINE_GAMEPLAY_TAG(OD_Magnitude_Reward_Weight, "OD.Magnitude.Reward.Weight");
	
}

namespace StateTags
{
	UE_DEFINE_GAMEPLAY_TAG(OD_State_Dead, "OD.State.Dead");
}

namespace TriggerTags
{
	UE_DEFINE_GAMEPLAY_TAG(OD_Init_FillVitals, "OD.Init.FillVitals");
	UE_DEFINE_GAMEPLAY_TAG(OD_Init_Regeneration, "OD.Init.Regeneration");
	UE_DEFINE_GAMEPLAY_TAG(OD_Init_MaxExp, "OD.Init.MaxExp");
	
	UE_DEFINE_GAMEPLAY_TAG(OD_Update_LevelUp, "OD.Update.LevelUp");
	
	UE_DEFINE_GAMEPLAY_TAG(OD_Transaction_FieldMobReward, "OD.Transaction.FieldMobReward");
}


namespace CrowdControlTags
{
}

namespace GameplayAbilityTags
{
	//Block
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Block, "OD.Ability.Block");
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Block_Combat, "OD.Ability.Block.Combat");

	//Init
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Init_Stats, "OD.Ability.Init.Stats");
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Init_FillVitals, "OD.Ability.Init.FillVitals");
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Init_Regeneration, "OD.Ability.Init.Regeneration");
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Init_MaxExp, "OD.Ability.Init.MaxExp");

	//State
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Combat_State_Death, "OD.Ability.Combat.State.Death");

	//Update
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Update_LevelUp, "OD.Ability.Update.Death");

	//Attack
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Combat_NormalAttack, "OD.Ability.Combat.NormalAttack");

	//Transaction
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_Transaction_FieldMobReward, "OD.Ability.Transaction.FieldMobReward");
	
}

namespace GameplayEffectTags
{
	//Init
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Init_FillVitals, "OD.Effect.Init.FillVitals");
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Init_Regeneration, "OD.Effect.Init.Regeneration");

	//Damage
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Combat_OnDamage, "OD.Effect.Combat.OnDamage");

	//Cost
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Skill_Cost, "OD.Effect.Skill.Cost");

	//Transaction
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Transaction_FieldReward, "OD.Effect.Transaction.FieldReward");
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Transaction_Gold, "OD.Effect.Transaction.Gold");
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Transaction_EXP, "OD.Effect.Transaction.Exp");

	//Boss Monster
	UE_DEFINE_GAMEPLAY_TAG(OD_Effect_Enrage_Status, "OD.Effect.Enrage.Status");
	
}

namespace GameplayAbilitySkillTags
{
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_TestSkill, "OD.Active.TestSkill");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_TestSkill_Cooldown, "OD.Active.TestSkill.Cooldown");

	//Valkyrie
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_001, "OD.Active.Valkyrie.001");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_001_Cooldown, "OD.Active.Valkyrie.001.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_002, "OD.Active.Valkyrie.002");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_002_Cooldown, "OD.Active.Valkyrie.002.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_003, "OD.Active.Valkyrie.003");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_003_Cooldown, "OD.Active.Valkyrie.003.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_004, "OD.Active.Valkyrie.004");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_004_Cooldown, "OD.Active.Valkyrie.004.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_005, "OD.Active.Valkyrie.005");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_005_Cooldown, "OD.Active.Valkyrie.005.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_006, "OD.Active.Valkyrie.006");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Valkyrie_006_Cooldown, "OD.Active.Valkyrie.006.Cooldown");

	// DarkMagician
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_001, "OD.Active.DarkMagician.001");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_001_Cooldown, "OD.Active.DarkMagician.001.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_002, "OD.Active.DarkMagician.002");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_002_Cooldown, "OD.Active.DarkMagician.002.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_003, "OD.Active.DarkMagician.003");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_003_Cooldown, "OD.Active.DarkMagician.003.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_004, "OD.Active.DarkMagician.004");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_004_Cooldown, "OD.Active.DarkMagician.004.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_005, "OD.Active.DarkMagician.005");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_005_Cooldown, "OD.Active.DarkMagician.005.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_006, "OD.Active.DarkMagician.006");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_DarkMagician_006_Cooldown, "OD.Active.DarkMagician.006.Cooldown");

	// Sniper
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_001, "OD.Active.Sniper.001");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_001_Cooldown, "OD.Active.Sniper.001.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_002, "OD.Active.Sniper.002");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_002_Cooldown, "OD.Active.Sniper.002.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_003, "OD.Active.Sniper.003");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_003_Cooldown, "OD.Active.Sniper.003.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_004, "OD.Active.Sniper.004");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_004_Cooldown, "OD.Active.Sniper.004.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_005, "OD.Active.Sniper.005");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_005_Cooldown, "OD.Active.Sniper.005.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_006, "OD.Active.Sniper.006");
	UE_DEFINE_GAMEPLAY_TAG(OD_Active_Sniper_006_Cooldown, "OD.Active.Sniper.006.Cooldown");
}

namespace NotifyTags
{
	//Hit
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Hit_1, "OD.Notify.Combat.Hit.1");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Hit_2, "OD.Notify.Combat.Hit.2");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Hit_3, "OD.Notify.Combat.Hit.3");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Hit_4, "OD.Notify.Combat.Hit.4");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Hit_5, "OD.Notify.Combat.Hit.5");

	//Spawn
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Spawn_1, "OD.Notify.Combat.Spawn.1");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Spawn_2, "OD.Notify.Combat.Spawn.2");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Spawn_3, "OD.Notify.Combat.Spawn.3");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Spawn_4, "OD.Notify.Combat.Spawn.4");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Combat_Spawn_5, "OD.Notify.Combat.Spawn.5");

	//Trigger
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Trigger_1, "OD.Notify.Trigger.1");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Trigger_2, "OD.Notify.Trigger.2");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Trigger_3, "OD.Notify.Trigger.3");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Trigger_4, "OD.Notify.Trigger.4");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_Trigger_5, "OD.Notify.Trigger.5");

	//ANS
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_ANS_1, "OD.Notify.ANS.1");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_ANS_2, "OD.Notify.ANS.2");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_ANS_3, "OD.Notify.ANS.3");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_ANS_4, "OD.Notify.ANS.4");
	UE_DEFINE_GAMEPLAY_TAG(OD_Notify_ANS_5, "OD.Notify.ANS.5");

}

namespace MonsterSkillAttackType
{
	//Monster Skill
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_AttackType_Skill, "OD.Ability.AttackType.Skill")
	UE_DEFINE_GAMEPLAY_TAG(OD_Ability_AttackType_Skill_AOE_SingleHit, "OD.Ability.AttackType.Skill.AOE.SingleHit")
	
}