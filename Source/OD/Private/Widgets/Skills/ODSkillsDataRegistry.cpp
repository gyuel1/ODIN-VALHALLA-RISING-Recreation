// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/ODSkillsDataRegistry.h"

#include "Character/ODPlayer.h"
#include "Components/Image.h"
#include "Components/ODSkillComponent.h"
#include "DataAssets/Ability/ODActiveSkillDA.h"
#include "Helper/ODDebugHelper.h"
#include "Helper/ODStaticFunction.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Active.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Collection.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Passive.h"


void UODSkillsDataRegistry::InitSkillsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	if (AODPlayer* Player = Cast<AODPlayer>(InOwningLocalPlayer->GetPlayerController(GetWorld())->GetPawn()))
	{
		PlayerSkillComponent = Player->GetODSkillComponent();
	}
	InitActiveSkillTab();
	InitPassiveSkillTab();
}

TArray<UODListSkillDataObject_Base*> UODSkillsDataRegistry::GetListSourceItemsBySelectedTabID(
	const FName& InSelectedTabID) const
{
	UODListSkillDataObject_Collection* const* FoundTabCollectionPtr = RegisteredSkillsTabCollections.FindByPredicate(
		[InSelectedTabID](UODListSkillDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
		);

	checkf(FoundTabCollectionPtr, TEXT("No Valid tab found under the ID %s"), *InSelectedTabID.ToString());

	UODListSkillDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;
	
	return FoundTabCollection->GetAllChildListData();
}

void UODSkillsDataRegistry::InitActiveSkillTab()
{
	UODListSkillDataObject_Collection* ActiveSkillTabCollection = NewObject<UODListSkillDataObject_Collection>();
	ActiveSkillTabCollection->SetDataID(FName("ActiveSkillTabCollection"));
	ActiveSkillTabCollection->SetDataDisplayName(FText::FromString(TEXT("액티브")));


	for (UODActiveSkillDA* ActiveSkill : PlayerSkillComponent->GetActiveSkills())
	{
		{
			UODListSkillDataObject_Active* NewActiveSkill = NewObject<UODListSkillDataObject_Active>();
			NewActiveSkill->SetDA_Skill(NewActiveSkill);
			NewActiveSkill->SetDataID(ActiveSkill->GetSkillInfo().SkillName);
			NewActiveSkill->SetDataDisplayName(FText::FromName(ActiveSkill->GetSkillInfo().SkillName));
			NewActiveSkill->SetSkillDescriptionText(ActiveSkill->GetSkillInfo().SkillDescription.Description);
			NewActiveSkill->SetSkillIcon(ODUIStaticFunc::LoadSynchronousSprintAndConvertSlateImage( ActiveSkill->GetSkillInfo().SkillIcon));
			NewActiveSkill->SetSkillGrade(ActiveSkill->GetSkillInfo().SkillGrade);
			NewActiveSkill->SetActivationType(ActiveSkill->GetSkillInfo().SkillType);
			NewActiveSkill->SetbIsUnlock(ActiveSkill->GetSkillInfo().bSkillUnlock);
			NewActiveSkill->SetSkillTag(ActiveSkill->GetTag());
			

			if (ActiveSkill->GetSkillInfo().SkillNormalDetailInfo.IsValidIndex(0))
			{
				for (FSkillDetailInfo DetailInfo : ActiveSkill->GetSkillInfo().SkillNormalDetailInfo)
				{
					NewActiveSkill->GetSkillNormalDetailInfo().Emplace(DetailInfo);
				}
			}

			if (ActiveSkill->GetSkillInfo().SkillSpecialDetailInfo.IsValidIndex(0))
			{
				for (FSkillDetailInfo DetailInfo : ActiveSkill->GetSkillInfo().SkillSpecialDetailInfo)
				{
					NewActiveSkill->GetSkillSpecialDetailInfo().Emplace(DetailInfo);
				}
			}
		
			ActiveSkillTabCollection->AddChildListData(NewActiveSkill);
		}
	}
	

	RegisteredSkillsTabCollections.Add(ActiveSkillTabCollection);
}

void UODSkillsDataRegistry::InitPassiveSkillTab()
{
	UODListSkillDataObject_Collection* PassiveSkillTabCollection = NewObject<UODListSkillDataObject_Collection>();
	PassiveSkillTabCollection->SetDataID(FName("PassiveSkillTabCollection"));
	PassiveSkillTabCollection->SetDataDisplayName(FText::FromString(TEXT("패시브")));

	

	RegisteredSkillsTabCollections.Add(PassiveSkillTabCollection);
}




