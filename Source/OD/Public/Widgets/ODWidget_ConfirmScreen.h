// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ODWidget_Activatable.h"
#include "Enum/ODUIEnumTypes.h"
#include "ODWidget_ConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class OD_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);

	
	UPROPERTY(Transient)
	FText ScreenTitle;

	UPROPERTY(Transient)
	FText ScreenMessage;

	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
	
};
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_ConfirmScreen : public UODWidget_Activatable
{
	GENERATED_BODY()

public:
	void InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallBack);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Message;

	// 동적으로 위젯을 생성하고 관리하기 위한 컨테이너
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> DynamicEntryBox_Buttons;
	
	
};
