// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/Slots/ODWidget_SkillSlotChangeScreen.h"

#include "Components/UniformGridPanel.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ODPlayerController.h"
#include "Widgets/Button/ODCommonWidget_Button_SlotChange.h"
#include "Widgets/HUD/ODWidget_HUD.h"
#include "Widgets/HUD/UserInput/ODWidget_Hud_UserInput.h"


void UODWidget_SkillSlotChangeScreen::NativeConstruct()
{

	Super::NativeConstruct();
	
	AODPlayerController* PC = Cast<AODPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (PC)
	{
		HudUserInput = PC->GetHUDWidget()->GetHudUserInput();
	}
	else
	{
		Debug::Print(TEXT("플레이어 컨트롤러 못가져옴"));
	}

	UODWidget_HUD* Hud = PC->GetHUDWidget();

	if (!Hud)
	{
		Debug::Print(TEXT("Hud Null"));
	}

	UODWidget_Hud_UserInput* HudUI = Hud->GetHudUserInput();

	if (!HudUI)
	{
		Debug::Print(TEXT("HudUI Null"));
	}
	
	SkillChangeSlotButtons = {
		SKillChangeSlotIndex1,
		SKillChangeSlotIndex2,
		SKillChangeSlotIndex3,
        SKillChangeSlotIndex4,
        SKillChangeSlotIndex5,
		SKillChangeSlotIndex6
	};

	
	for (int i = 0; i < SkillChangeSlotButtons.Num(); ++i)
	{
		if (SkillChangeSlotButtons[i])
		{
			SkillChangeSlotButtons[i]->SetButtonText(FText::AsNumber(i + 1));
			
			SkillChangeSlotButtons[i]->SetSKillHudActive(HudUserInput->Get_SKillHudActives()[i]);
			
			int index = i;
			SkillChangeSlotButtons[i]->OnClicked().AddLambda([this, index]()
			{
				OnSkillSlotChangeClicked(index);
			});
		}
	}

	
}

void UODWidget_SkillSlotChangeScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UODWidget_SkillSlotChangeScreen::OnSkillSlotChangeClicked(int ClickedIndex)
{
	for (int i = 0; i < SkillChangeSlotButtons.Num(); ++i)
	{
		if (SkillChangeSlotButtons[i] && i != ClickedIndex)
		{
			// 다른 버튼들은 항상 해제
			SkillChangeSlotButtons[i]->SetIsSelected(false);
		}
	}

	if (SkillChangeSlotButtons.IsValidIndex(ClickedIndex))
	{
		UCommonButtonBase* ClickedButton = SkillChangeSlotButtons[ClickedIndex];

		if (ClickedButton->GetSelected())
		{
			CurrentSelectedIndex = ClickedIndex;
		}
		else
		{
			CurrentSelectedIndex = INDEX_NONE;
		}
			
	}
}
