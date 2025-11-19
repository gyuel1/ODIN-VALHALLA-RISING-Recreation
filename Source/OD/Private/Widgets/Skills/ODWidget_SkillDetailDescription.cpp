// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/ODWidget_SkillDetailDescription.h"
#include "CommonRichTextBlock.h"
#include "PaperSprite.h"
#include "Components/Image.h"
#include "Helper/ODStaticFunction.h"
#include "Widgets/UIDataAsset/ODDataAsset_BuffIcon.h"


void UODWidget_SkillDetailDescription::SetBuffTexture(EBuffAttribute BuffAttribute)
{
	if (const TSoftObjectPtr<UPaperSprite>* FoundSpriteSoftPtr = DA_BuffIcon->GetBuffInfo().BuffSprite.Find(BuffAttribute))
	{
		if (UPaperSprite* FoundSprite = FoundSpriteSoftPtr->LoadSynchronous())
		{
			Image_BuffIcon->SetBrush(ODUIStaticFunc::LoadSynchronousSprintAndConvertSlateImage(FoundSprite));
		}
	}
}

void UODWidget_SkillDetailDescription::SetSkillDescription(const FText& SkillDescription)
{
	CommonRichTextBlock_SkillDescription->SetText(SkillDescription);
}
