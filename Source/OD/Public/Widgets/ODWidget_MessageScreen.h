// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/ODUIEnumTypes.h"
#include "Widgets/ODWidget_Activatable.h"
#include "ODWidget_MessageScreen.generated.h"

class UEditableText;
/**
 * 
 */
class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FMessageScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType MessageScreenButtonType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class OD_API UMessageScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UMessageScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText);
	static UMessageScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText);
	static UMessageScreenInfoObject* CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText);

	
	UPROPERTY(Transient)
	FText ScreenTitle;

	UPROPERTY(Transient)
	FText ScreenMessage;

	UPROPERTY(Transient)
	FText EditHintText;

	UPROPERTY(Transient)
	FText EditText;

	UPROPERTY(Transient)
	TArray<FMessageScreenButtonInfo> AvailableScreenButtons;
	
};
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_MessageScreen : public UODWidget_Activatable
{
	GENERATED_BODY()

public:
	void InitMessageScreen(UMessageScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType, const FText&)> ClickedButtonCallBack);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Message;

	// 동적으로 위젯을 생성하고 관리하기 위한 컨테이너
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> DynamicEntryBox_Buttons;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> EditableText;
};