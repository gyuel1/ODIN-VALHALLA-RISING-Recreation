// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/ODUIEnumTypes.h"
#include "Helper/ODMacros.h"
#include "UObject/NoExportTypes.h"
#include "ODListDataObject_Base.generated.h"

enum class EOptionsListDataModifyReason : uint8;
/**
 * 
 */
UCLASS(Abstract)
class OD_API UODListDataObject_Base : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UODListDataObject_Base*, EOptionsListDataModifyReason)
	FOnListDataModifiedDelegate OnListDataModified;
	
	OD_DATA_ACCESSOR(FName, DataID)
	OD_DATA_ACCESSOR(FText, DataDisplayName)
	OD_DATA_ACCESSOR(FText, DescriptionRichText)
	OD_DATA_ACCESSOR(FText, DisabledRichText)
	OD_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	OD_DATA_ACCESSOR(UODListDataObject_Base*, ParentData)

	void InitDataObject();
	virtual TArray<UODListDataObject_Base*> GetAllChildListData() const {return TArray<UODListDataObject_Base*>();}
	virtual bool HasAnyChildListData() const {return false;}
	void SetShouldApplySettingImmediately(bool bShouldAllyRightAway) {bShouldApplyChangeImmediatly = bShouldAllyRightAway;}

	// Resetting 데이터 함수
	virtual bool HasDefaultValue() const {return false;}
	virtual bool CanResetBackToDefaultValue() const {return false;}
	virtual bool TryResetBackToDefaultValue() {return false;}

protected:
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(UODListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UODListDataObject_Base* ParentData;

	bool bShouldApplyChangeImmediatly = false;
};
