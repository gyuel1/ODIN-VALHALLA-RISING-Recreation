// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ODWidget_HudRightDefaultStack.h"
#include "Widgets/ODWidget_Activatable.h"
#include "ODWidget_HUD.generated.h"

class UODWidget_LevelUp;
struct FOnAttributeChangeData;
class UTextBlock;
class UODWidget_HudRightDefaultStack;
class UODWidget_Credits;
class UODHudGauge;
class UAbilitySystemComponent;
class UODWidget_Hud_UserInput;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_HUD : public UODWidget_Activatable
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UODWidget_Hud_UserInput> GetHudUserInput() const {return HudRightDefaultWidget->GetHudUserInput();}
	FORCEINLINE TWeakObjectPtr<UODWidget_HudRightDefaultStack> GetHudRightDefaultWidget() const {return HudRightDefaultWidget;}
	
protected:
	virtual void NativeConstruct() override;
	void TryInitializeValues();
	void OnLevelChanged(const FOnAttributeChangeData& ChangeData) const;
	void UpdateExpText() const;
	void OnExpChanged(const FOnAttributeChangeData& ChangeData);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> PlayerAbilitySystemComponent;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UODHudGauge> HPGauge;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UODHudGauge> MPGauge;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UODHudGauge> ExpGauge;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UODWidget_Credits> Credits;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UODWidget_HudRightDefaultStack> HudRightDefaultWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> TextBlock_Level;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> TextBlock_Exp;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UODWidget_LevelUp> WidgetLevelUp;
	
};
