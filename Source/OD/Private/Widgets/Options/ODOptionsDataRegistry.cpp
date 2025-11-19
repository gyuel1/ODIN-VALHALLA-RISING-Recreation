// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ODOptionsDataRegistry.h"

#include "Setting/ODGameUserSettings.h"
#include "Widgets/Options/ODOtionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ODListDataObject_String.h"


#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FODOtionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UODGameUserSettings,SetterOrGetterFuncName))

void UODOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGamePlayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UODListDataObject_Base*> UODOptionsDataRegistry::GetListSourceItemsBySelectedTabID(
	const FName& InSelectedTabID) const
{
	UODListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UODListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
		);

	checkf(FoundTabCollectionPtr, TEXT("No Valid tab found under the ID %s"), *InSelectedTabID.ToString());

	UODListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UODListDataObject_Base*> AllChildListItems;

	for (UODListDataObject_Base* SubChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		AllChildListItems.Emplace(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UODOptionsDataRegistry::FindChildListDataRecursively(UODListDataObject_Base* InParentData,
	TArray<UODListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UODListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Emplace(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UODOptionsDataRegistry::InitGamePlayCollectionTab()
{
	UODListDataObject_Collection* GamePlayTabCollection = NewObject<UODListDataObject_Collection>();
	GamePlayTabCollection->SetDataID(FName("GamePlayTabCollection"));
	GamePlayTabCollection->SetDataDisplayName(FText::FromString(TEXT("게임")));

	// TODO : 메크로화 시킴
	//TSharedPtr<FODOtionsDataInteractionHelper> ConstructHelper = MakeShared<FODOtionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UODGameUserSettings, GetCurrentPlayerInfoVisibility));
	
	// 캐릭터 HP/MP바 표시
	{
		UODListDataObject_String* PlayerInfoVisibility = NewObject<UODListDataObject_String>();
		PlayerInfoVisibility->SetDataID(FName("PlayerInfoVisibility"));
		PlayerInfoVisibility->SetDataDisplayName(FText::FromString(TEXT("플레이어 정보 표시 (머리 위)")));
		PlayerInfoVisibility->AddDynamicOption(TEXT("MarkAll"), FText::FromString(TEXT("모두 표기")));
		PlayerInfoVisibility->AddDynamicOption(TEXT("MarkOnly"), FText::FromString(TEXT("나만 표기")));
		PlayerInfoVisibility->AddDynamicOption(TEXT("MarkOtherPlayer"), FText::FromString(TEXT("다른 플레이어만 표기")));
		PlayerInfoVisibility->AddDynamicOption(TEXT("AllOff"), FText::FromString(TEXT("모두 끄기")));
		PlayerInfoVisibility->SetDefaultValueFromString(TEXT("MarkAll"));
		PlayerInfoVisibility->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentPlayerInfoVisibility));
		PlayerInfoVisibility->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentPlayerInfoVisibility));
		PlayerInfoVisibility->SetShouldApplySettingImmediately(true);

		GamePlayTabCollection->AddChildListData(PlayerInfoVisibility);
	}
	
	RegisteredOptionsTabCollections.Emplace(GamePlayTabCollection);
}

void UODOptionsDataRegistry::InitAudioCollectionTab()
{
	UODListDataObject_Collection* AudioTabCollection = NewObject<UODListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("사운드")));

	
	// 전체음
	{
		UODListDataObject_Scalar* OverAllVolume = NewObject<UODListDataObject_Scalar>();
		OverAllVolume->SetDataID(FName("OverAllVolume"));
		OverAllVolume->SetDataDisplayName(FText::FromString(TEXT("전체음")));
		OverAllVolume->SetDisplayValueRange(TRange<float>(0.f,1.f));
		OverAllVolume->SetOutputValueRange(TRange<float>(0.f,2.f));
		OverAllVolume->SetSliderStepSize(0.01f);
		OverAllVolume->SetDefaultValueFromString(LexToString(1.f));
		OverAllVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
		OverAllVolume->SetNumberFormattingOptions(UODListDataObject_Scalar::NoDecimal());
		OverAllVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverAllVolume));
		OverAllVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverAllVolume));
		OverAllVolume->SetShouldApplySettingImmediately(true);
		
		AudioTabCollection->AddChildListData(OverAllVolume);
	}

	// 배경음
	{
		UODListDataObject_Scalar* BGMVolume = NewObject<UODListDataObject_Scalar>();
		BGMVolume->SetDataID(FName("BGMVolume"));
		BGMVolume->SetDataDisplayName(FText::FromString(TEXT("배경음")));
		BGMVolume->SetDisplayValueRange(TRange<float>(0.f,1.f));
		BGMVolume->SetOutputValueRange(TRange<float>(0.f,2.f));
		BGMVolume->SetSliderStepSize(0.01f);
		BGMVolume->SetDefaultValueFromString(LexToString(1.f));
		BGMVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
		BGMVolume->SetNumberFormattingOptions(UODListDataObject_Scalar::NoDecimal());
		BGMVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetBGMVolume));
		BGMVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetBGMVolume));
		BGMVolume->SetShouldApplySettingImmediately(true);
		
		AudioTabCollection->AddChildListData(BGMVolume);
	}
	
	RegisteredOptionsTabCollections.Emplace(AudioTabCollection);
}

void UODOptionsDataRegistry::InitVideoCollectionTab()
{
	UODListDataObject_Collection* VideoTabCollection = NewObject<UODListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("비디오")));

	RegisteredOptionsTabCollections.Emplace(VideoTabCollection);
}

void UODOptionsDataRegistry::InitControlCollectionTab()
{
	UODListDataObject_Collection* ControlTabCollection = NewObject<UODListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("조작")));

	RegisteredOptionsTabCollections.Emplace(ControlTabCollection);
}
