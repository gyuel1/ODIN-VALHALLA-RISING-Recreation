// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Component/ODCommonTabListWidgetBase.h"
#include "Widgets/Component/ODCommonButtonBase.h"
#include "Editor/WidgetCompilerLog.h"


void UODCommonTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UODCommonButtonBase* FoundButton = Cast<UODCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

#if WITH_EDITOR
void UODCommonTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The Variable TabButtonEntryWidgetClass Has no Valid entry. ") +
			GetClass()->GetName() +
			TEXT(" Needs a valid entry widget class to function property")));
	}
}
#endif