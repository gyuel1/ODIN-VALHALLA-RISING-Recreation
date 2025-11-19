// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enum/ODUIEnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ODUISubsystem.generated.h"

class UODWidget_Activatable;
class UODWidget_PrimaryLayout;
enum class EPlayerJob : uint8;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush,
	Failed
};
/**
 * 
 */
UCLASS()
class OD_API UODUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UODUISubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UODWidget_PrimaryLayout* InCreateWidget);
	
	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UODWidget_Activatable> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, UODWidget_Activatable*)> AsyncPushStateCallBack);

	void PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType,  const FText& InScreenTitle, const FText& InScreenMessage,
		TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallBack);

	void PushMessageScreenToModalStackAsync(EConfirmScreenType InScreenType,  const FText& InScreenTitle, const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText,
		TFunction<void(EConfirmScreenButtonType, const FText&)> ButtonClickedCallBack);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetWidgetPlayerName() {return WidgetPlayerName;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetWidgetPlayerName(const FString& Name) {WidgetPlayerName = Name;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPlayerJob GetPlayerJob() {return PlayerJob;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPlayerJob(const EPlayerJob& Job) {PlayerJob = Job;}

private:
	UPROPERTY(Transient)
	UODWidget_PrimaryLayout* CreatedPrimaryLayout;
	
	UPROPERTY()
	FString WidgetPlayerName;

	UPROPERTY()
	EPlayerJob PlayerJob;

	
};
