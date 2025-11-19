// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Init/OD_GE_Init_MaxExp.h"

#include "AbilitySystem/ODPlayerAttributeSet.h"


UOD_GE_Init_MaxExp::UOD_GE_Init_MaxExp()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	static ConstructorHelpers::FObjectFinder<UCurveTable> CT(
		TEXT("/Script/Engine.CurveTable'/Game/_BP/DataTable/CurveTable/CV_MaxExp.CV_MaxExp'"));
	UCurveTable* CurveTable = CT.Succeeded() ? CT.Object : nullptr;
	
	FGameplayModifierInfo Mod;
	Mod.Attribute  = UODPlayerAttributeSet::GetMaxExpAttribute();
	Mod.ModifierOp = EGameplayModOp::Override;

	FScalableFloat SF;
	SF.Value = 1.f;
	SF.Curve.CurveTable = CurveTable;
	SF.Curve.RowName = FName("MaxExp");

	Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(SF);

	Modifiers.Add(Mod);
}
