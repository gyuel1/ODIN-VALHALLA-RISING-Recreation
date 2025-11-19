// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ODCommonButtonBase.generated.h"

/**
 * 
 */
class UCommonTextBlock;
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText Text);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnClicked() override;

	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OD Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OD Button", meta = (AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText = false;
};
