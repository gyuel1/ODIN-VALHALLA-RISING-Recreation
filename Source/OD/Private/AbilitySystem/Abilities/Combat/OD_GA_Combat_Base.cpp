// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/OD_GA_Combat_Base.h"


int32 UOD_GA_Combat_Base::PickIndexDeterministic(const FGameplayAbilityActivationInfo& Info, int32 Count)
{
	const FPredictionKey Key = Info.GetActivationPredictionKey();
	if (Key.IsValidKey())
	{
		uint32 Seed = 0x9E3779B9u;
		Seed ^= (uint32)Key.Current;
		Seed ^= ((uint32)Key.Base * 2654435761u);
		Seed ^= FCrc::StrCrc32(*GetClass()->GetPathName());

		FRandomStream S(Seed);
		(void)S.GetUnsignedInt();
		return S.RandHelper(Count);
	}
	return FMath::RandHelper(Count);
}
