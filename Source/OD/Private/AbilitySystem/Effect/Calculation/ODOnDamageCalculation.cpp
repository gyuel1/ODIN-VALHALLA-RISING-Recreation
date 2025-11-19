// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Effect/Calculation/ODOnDamageCalculation.h"
#include "AbilitySystem/ODAttributeSet.h"
#include <cmath>
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;

class FOnDamageSourceCapture
{
public:
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);				//공격력
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageIncrease);			//피해증가
	DECLARE_ATTRIBUTE_CAPTUREDEF(Accuracy);					//명중률
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);			//크리티컬 확률
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);			//크리티컬 데미지

	FOnDamageSourceCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, AttackPower,     Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, DamageIncrease,  Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, Accuracy,        Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, CriticalChance,  Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, CriticalDamage,  Source, false);
	}
};

class FOnDamageTargetCapture
{
public:
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health); 						//현재 체력
	DECLARE_ATTRIBUTE_CAPTUREDEF(Evasion);						//회피율
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);						//방어력
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);				//받는피해 감소

	DECLARE_ATTRIBUTE_CAPTUREDEF(EvasionProc);					//회피 판정
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalProc);					//크리티컬 판정

	FOnDamageTargetCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, Health,          Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, Evasion,         Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, Defense,         Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, DamageReduction, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, EvasionProc, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UODAttributeSet, CriticalProc, Target, false);

		
	}
};


static const FOnDamageSourceCapture& GetSourceCapture()
{
	static FOnDamageSourceCapture SourceCapture;
	return SourceCapture;
}

static const FOnDamageTargetCapture& GetTargetCapture()
{
	static FOnDamageTargetCapture TargetCapture;
	return TargetCapture;
}


UODOnDamageCalculation::UODOnDamageCalculation()
{
	// Source
	RelevantAttributesToCapture.Add(GetSourceCapture().AttackPowerDef);     // 공격력
	RelevantAttributesToCapture.Add(GetSourceCapture().DamageIncreaseDef);  // 피해 증가
	RelevantAttributesToCapture.Add(GetSourceCapture().AccuracyDef);        // 명중률
	RelevantAttributesToCapture.Add(GetSourceCapture().CriticalChanceDef);  // 크리티컬 확률
	RelevantAttributesToCapture.Add(GetSourceCapture().CriticalDamageDef);  // 크리티컬 데미지

	// Target
	RelevantAttributesToCapture.Add(GetTargetCapture().HealthDef);          // 현재 체력
	RelevantAttributesToCapture.Add(GetTargetCapture().DefenseDef);         // 방어력
	RelevantAttributesToCapture.Add(GetTargetCapture().EvasionDef);         // 회피율
	RelevantAttributesToCapture.Add(GetTargetCapture().DamageReductionDef); // 받는 피해 감소

}

void UODOnDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters Eval;
	Eval.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	Eval.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//Source
	float SourceAttack = 0.f;
	float SourceDamageIncrease = 0.f;
	float SourceAccuracy = 0.f;
	float SourceCriticalChance = 0.f;
	float SourceCriticalDamage = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSourceCapture().AttackPowerDef,     Eval, SourceAttack);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSourceCapture().DamageIncreaseDef,     Eval, SourceDamageIncrease);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSourceCapture().AccuracyDef,     Eval, SourceAccuracy);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSourceCapture().CriticalChanceDef,     Eval, SourceCriticalChance);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSourceCapture().CriticalDamageDef,     Eval, SourceCriticalDamage);
	
	//Target
	float TargetHealth = 0.f;
	float TargetDefense = 0.f;
	float TargetEvasion = 0.f;
	float TargetDamageReduction = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTargetCapture().HealthDef,  Eval, TargetHealth);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTargetCapture().DefenseDef,  Eval, TargetDefense);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTargetCapture().EvasionDef,  Eval, TargetEvasion);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTargetCapture().DamageReductionDef,  Eval, TargetDamageReduction);

	//Target Proc
	float TargetEvasionProc = 0.f;
	float TargetCriticalProc = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTargetCapture().EvasionProcDef,  Eval, TargetEvasionProc);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTargetCapture().CriticalProcDef,  Eval, TargetCriticalProc);

	//SkillInfo
	float SkillDamagePercent = Spec.GetSetByCallerMagnitude(OD_Magnitude_DamagePercent , false , 0.f);
	float SkillAttackCount = Spec.GetSetByCallerMagnitude(OD_Magnitude_AttackCount , false , 0.f);
	
	//========================================Calculation========================================//

	//회피판정 계산
	const float CurrentEvasion = TargetEvasion - SourceAccuracy;
	if (CurrentEvasion >= 0.f && FMath::FRandRange(0.f,100.f) <= CurrentEvasion)
	{
		TargetEvasionProc = 1.f;
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UODAttributeSet::GetEvasionProcAttribute(),
				EGameplayModOp::Override,
				TargetEvasionProc));
	}
	else
	{
		float Result = 0.f;
		SourceDamageIncrease = FMath::Clamp(1.f + SourceDamageIncrease / 100.f,0.f,2.f);			//피해증가
		TargetDamageReduction = FMath::Clamp(1.f - TargetDamageReduction/ 100.f,0.f,1.f);			//받는 피해 감소

		// 공격력 * 피해증가 - 방어력
		Result = ((SourceAttack * SourceDamageIncrease) - TargetDefense) * TargetDamageReduction;

		//데미지 = 데미지 * 스킬계수 / 스킬타수
		Result = (Result * (SkillDamagePercent / 100.f)) / SkillAttackCount;

		if (Result < 1.f)
			Result = 1.f;

		//크리티컬 확인 반영
		if (Result >= 0 && FMath::FRandRange(0.f,100.f) <= SourceCriticalChance)
		{
			SourceCriticalDamage = 1.f + SourceCriticalDamage / 100.f;
			Result *= SourceCriticalDamage;
			
			TargetCriticalProc = 1.f;
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(
					UODAttributeSet::GetCriticalProcAttribute(),
					EGameplayModOp::Override,
					TargetCriticalProc));
		}

		//소수버림
		Result = std::trunc(Result);
		
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UODAttributeSet::GetHealthAttribute(),
				EGameplayModOp::Additive,
				-Result));
		
	}
}
