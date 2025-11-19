// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Component/ODCommonListView.h"
#include "ODCommonSkillListView.generated.h"

/**
 * 
 */
class UODDA_DataSkillListEntryMapping;
UCLASS()
class OD_API UODCommonSkillListView : public UODCommonListView
{
	GENERATED_BODY()
	
	
protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;

#if WITH_EDITOR
private:
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	
	UPROPERTY(EditAnywhere, Category = "OD List View Setting")
	UODDA_DataSkillListEntryMapping* DataListEntryMapping;
};
