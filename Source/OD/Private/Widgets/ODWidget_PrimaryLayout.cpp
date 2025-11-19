// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ODWidget_PrimaryLayout.h"

#include "Helper/ODDebugHelper.h"


UCommonActivatableWidgetContainerBase* UODWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can't not Widget stack by Tag : %s"),*InTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InTag);
}

void UODWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "OD.WidgetStack")) FGameplayTag InStackTag,
	UCommonActivatableWidgetContainerBase* InStack)
{
	// IsDesignTime() 게임실행중에는 false 뷰포트에서 위젯을 볼떈 True를 반환, 에디터와 런타임 환경 분리
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}

	
}
