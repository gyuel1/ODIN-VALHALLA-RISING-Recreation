// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/ODWidget_HUD.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ODPlayerAttributeSet.h"
#include "Components/TextBlock.h"
#include "Helper/ODDebugHelper.h"
#include "Widgets/Gauge/ODHudGauge.h"
#include "Widgets/HUD/ODWidget_LevelUp.h"
#include "Widgets/HUD/UserInput/ODWidget_Hud_UserInput.h"


void UODWidget_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	TryInitializeValues();


	
	
}

void UODWidget_HUD::TryInitializeValues()
{
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return;

	PlayerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);
	if (!PlayerAbilitySystemComponent.IsValid())
	{
		// ASC 준비 안 됐으면 0.05초 후 재시도
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UODWidget_HUD::TryInitializeValues, 0.05f, false);
		return;
	}

	if (HPGauge) HPGauge->SetAndBoundToGameplayAttribute(PlayerAbilitySystemComponent.Get(), UODAttributeSet::GetHealthAttribute(), UODAttributeSet::GetMaxHealthAttribute());
	if (MPGauge) MPGauge->SetAndBoundToGameplayAttribute(PlayerAbilitySystemComponent.Get(), UODAttributeSet::GetManaAttribute(), UODAttributeSet::GetMaxManaAttribute());
	if (ExpGauge) ExpGauge->SetAndBoundToGameplayAttribute(PlayerAbilitySystemComponent.Get(), UODPlayerAttributeSet::GetCurrentExpAttribute(), UODPlayerAttributeSet::GetMaxExpAttribute());
	if (TextBlock_Level)
	{
		PlayerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UODAttributeSet::GetLevelAttribute()).AddUObject(this, &ThisClass::OnLevelChanged);
	}
	if (TextBlock_Exp)
	{
		PlayerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UODPlayerAttributeSet::GetCurrentExpAttribute()).AddUObject(this, &ThisClass::OnExpChanged);
		PlayerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UODPlayerAttributeSet::GetMaxExpAttribute()).AddUObject(this, &ThisClass::OnExpChanged);
	}
}


void UODWidget_HUD::OnLevelChanged(const FOnAttributeChangeData& ChangeData) const
{
	const int32 NewLevel = static_cast<int32>(ChangeData.NewValue);
	
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
	WidgetLevelUp->LevelUp(NewLevel);
}

void UODWidget_HUD::UpdateExpText() const
{
	const float CurrentExp = PlayerAbilitySystemComponent->GetNumericAttribute(UODPlayerAttributeSet::GetCurrentExpAttribute());
	const float MaxExp = PlayerAbilitySystemComponent->GetNumericAttribute(UODPlayerAttributeSet::GetMaxExpAttribute());

	float Percent = 0.0f;

	if (MaxExp > 0.0f)
	{
		Percent = FMath::Clamp((CurrentExp / MaxExp) * 100.0f, 0.0f, 100.0f);
	}

	FString PercentString = FString::Printf(TEXT("%.4f%%"), Percent);
	TextBlock_Exp->SetText(FText::FromString(PercentString));

	
}



void UODWidget_HUD::OnExpChanged(const FOnAttributeChangeData& ChangeData)
{
	UpdateExpText();
}
