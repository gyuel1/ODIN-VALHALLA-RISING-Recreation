// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Ability/ODSkillDA.h"


UODSkillDA::UODSkillDA()
	:Super()
{
}

void UODSkillDA::UnlockSkill()
{
	SkillInfo.bSkillUnlock = true;
}
