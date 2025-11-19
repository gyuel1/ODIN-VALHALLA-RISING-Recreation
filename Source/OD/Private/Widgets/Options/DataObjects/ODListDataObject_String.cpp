// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ODListDataObject_String.h"

#include "Helper/ODDebugHelper.h"
#include "Widgets/Options/ODOtionsDataInteractionHelper.h"


void UODListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableOptionsStringArray.Emplace(InStringValue);
	AvailableOptionsTextArray.Emplace(InDisplayText);
}

void UODListDataObject_String::AdvanceToNextOption()
{
	// 둘중 하나라도 비어있으면
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;

	const bool bIsNextIndexValid = AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay);

	if (bIsNextIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	 if (DataDynamicSetter)
	 {
	 	DataDynamicSetter->SetValueFromString(CurrentStringValue);
	 	Debug::Print(TEXT("동적데이터세터 사용됨 마지막 게터값 : ") + DataDynamicGetter->GetValueAsString());
	 	NotifyListDataModified(this);
	 }
	
}

void UODListDataObject_String::BackToPreviousOption()
{
	// 둘중 하나라도 비어있으면
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	
	const bool bIsPreviousIndexValid = AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay);

	if (bIsPreviousIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[PreviousIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		Debug::Print(TEXT("동적데이터세터 사용됨 마지막 게터값 : ") + DataDynamicGetter->GetValueAsString());
		NotifyListDataModified(this);
	}
}

void UODListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	// 데이터가 있을경우 그값으로 덮어씀
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
	
}

bool UODListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	const int32 CurrentFoundIndex = AvailableOptionsStringArray.IndexOfByKey(InStringValue);

	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];

		return true;
	}

	return false;
}


bool UODListDataObject_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UODListDataObject_String::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();

		TrySetDisplayTextFromStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			
			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
			
			return true;
		}
	}
	
	return false;
}

