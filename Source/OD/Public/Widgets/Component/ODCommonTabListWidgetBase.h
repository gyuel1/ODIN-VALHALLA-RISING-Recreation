// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "ODCommonTabListWidgetBase.generated.h"

class UODCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODCommonTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);
	

private:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	
	// 디버그 에디터상 탭 수량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OD Tab List Setting", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OD Tab List Setting", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UODCommonButtonBase> TabButtonEntryWidgetClass;
	
};
