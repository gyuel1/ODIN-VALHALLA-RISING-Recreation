// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/UserInput/ODWidget_Hud_UserInput.h"

#include "Character/ODPlayer.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ODTargetingAndAttackComponent.h"
#include "Enum/ODEnumCharacterJob.h"
#include "Helper/ODStaticFunction.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ODPlayerState.h"
#include "Widgets/Component/ODCommonButtonBase.h"
#include "Widgets/Skills/Slots/ODWidget_SkillHudActive.h"




void UODWidget_Hud_UserInput::NativeConstruct()
{
	Super::NativeConstruct();
	
	SKillHudActives = {
		SKillHudActive1,
		SKillHudActive2,
		SKillHudActive3,
		SKillHudActive4,
		SKillHudActive5,
		SKillHudActive6
	};

	for (int i = 0; i < SKillHudActives.Num(); ++i)
	{
		if (SKillHudActives[i])
		{
			SKillHudActives[i]->SetSKillNumberText(FText::AsNumber(i + 1));
		}
	}


	Player = Cast<AODPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	Button_Jump->OnClicked.AddDynamic(this,&ThisClass::OnClickButtonJump);
	Button_Attack->OnClicked.AddDynamic(this,&ThisClass::OnClickButtonAttack);
	Button_Auto->OnClicked().AddUObject(this, &ThisClass::OnClickButtonAuto);
	Player->OnAutoHuntChanged.AddDynamic(this,&ThisClass::OnAutoHuntChanged);

	SetAttackTexture();
}

void UODWidget_Hud_UserInput::OnClickButtonJump()
{
	Player->Jump();
}

void UODWidget_Hud_UserInput::OnClickButtonAttack()
{
	Player->GetTargetingAndAttackComponent()->TargetMove();
}

void UODWidget_Hud_UserInput::OnClickButtonAuto()
{
	Player->SetAutoHuntToggledByUser(!Player->GetAutoHuntToggledByUser());

	if (Player->GetAutoHuntToggledByUser())
	{
		Player->StartAutoHunt();
	}
	else
	{
		Player->StopAutoHunt();
	}
}

void UODWidget_Hud_UserInput::OnAutoHuntChanged(bool bIsAutoHunt)
{
	Image_AutoSelect->SetVisibility(bIsAutoHunt ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UODWidget_Hud_UserInput::SetAttackTexture()
{
	if (AODPlayerState* PS = Cast<AODPlayerState>(Player->GetPlayerState()))
	{
		EPlayerJob PlayerJob = PS->GetPlayerJob();

		switch (PlayerJob)
		{
		case EPlayerJob::Sorceress:
			Image_Attack->SetBrush(ODUIStaticFunc::LoadSynchronousSprintAndConvertSlateImage(Image_PaperSpriteJob[0].LoadSynchronous()));
			return;
		case EPlayerJob::ShieldMaiden:
			Image_Attack->SetBrush(ODUIStaticFunc::LoadSynchronousSprintAndConvertSlateImage(Image_PaperSpriteJob[1].LoadSynchronous()));
			return;
		case EPlayerJob::Rogue:
			Image_Attack->SetBrush(ODUIStaticFunc::LoadSynchronousSprintAndConvertSlateImage(Image_PaperSpriteJob[2].LoadSynchronous()));
		}
	}
}
