// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/Slots/ODWidget_SkillSlot_SkillBook.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "DataAssets/Ability/ODSkillDA.h"
#include "Enum/ODEnumGrade.h"
#include "Helper/ODDebugHelper.h"
#include "Helper/ODStaticFunction.h"
#include "Widgets/ODDataAsset_GradeInside.h"


void UODWidget_SkillSlot_SkillBook::SetSizeBox_SkillBook()
{
	SizeBox_SkillBackGround->SetWidthOverride(80.0f);
	SizeBox_SkillBackGround->SetHeightOverride(80.0f);
	SizeBox_SkillIcon->SetWidthOverride(55.0f);
	SizeBox_SkillIcon->SetHeightOverride(55.0f);
}

void UODWidget_SkillSlot_SkillBook::SetSizeBox_SkillDetailView()
{
	SizeBox_SkillBackGround->SetWidthOverride(120.0f);
	SizeBox_SkillBackGround->SetHeightOverride(120.0f);
	SizeBox_SkillIcon->SetWidthOverride(85.0f);
	SizeBox_SkillIcon->SetHeightOverride(85.0f);
}

void UODWidget_SkillSlot_SkillBook::SetSkillActivationAndGrade(ODSkillActivation ActivationType, EGradeType SkillGrade)
{
	if (ActivationType == ODSkillActivation::Active)
	{
		if (const TSoftObjectPtr<UPaperSprite>* FoundSpriteSoftPtr = DA_GradeInside->GetGradeTexture().CircleSlotTextures.Find(SkillGrade))
		{
			if (UPaperSprite* FoundSprite = FoundSpriteSoftPtr->LoadSynchronous())
			{
				Image_SlotGradeColor->SetBrush(ODUIStaticFunc::LoadSynchronousSprintAndConvertSlateImage(FoundSprite));
				
				if (!SoftImage_BackGroundActive)
				{
					return;
				}
	
				UTexture2D* LoadedTexture = SoftImage_BackGroundActive.LoadSynchronous();

				if (LoadedTexture)
				{
					Image_SlotBackGround->SetBrushFromTexture(LoadedTexture);
				}
			
			}
		}
	}

	else if (ActivationType == ODSkillActivation::Passive)
	{
		if (const TSoftObjectPtr<UPaperSprite>* FoundSpriteSoftPtr = DA_GradeInside->GetGradeTexture().SquareSlotTextures.Find(SkillGrade))
		{
			if (UPaperSprite* FoundSprite = FoundSpriteSoftPtr->LoadSynchronous())
			{
				Image_SlotGradeColor->SetBrush(ODUIStaticFunc::LoadSynchronousSprintAndConvertSlateImage(FoundSprite));
				
				if (!SoftImage_BackGroundPassive)
				{
					return;
				
				}
	
				UTexture2D* LoadedTexture = SoftImage_BackGroundPassive.LoadSynchronous();

				if (LoadedTexture)
				{
					Image_SlotBackGround->SetBrushFromTexture(LoadedTexture);
				}
				
			}
		}
	}
	
}

