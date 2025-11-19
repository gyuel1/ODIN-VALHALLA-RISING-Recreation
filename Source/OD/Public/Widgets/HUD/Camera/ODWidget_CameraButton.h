// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Component/ODCommonButtonBase.h"
#include "ODWidget_CameraButton.generated.h"

/**
 * 
 */

class AODPlayer;
class UTextBlock;

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	NormalView,
	QuarterView,
	DefaultView
};

UCLASS()
class OD_API UODWidget_CameraButton : public UODCommonButtonBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnClicked() override;
	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	ECameraMode CurrentMode = ECameraMode::NormalView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	int32 MaxIndex = 3;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ButtonText;

	UPROPERTY(Transient)
	TWeakObjectPtr<AODPlayer> PlayerCharacter;
};
