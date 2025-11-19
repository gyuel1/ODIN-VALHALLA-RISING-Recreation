// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODCharacterSelectManager.generated.h"

class UODWidget_Activatable;
class AODWidgetCharacter_Select;
class UODWidget_PrimaryLayout;

UCLASS()
class OD_API AODCharacterSelectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AODCharacterSelectManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void OnCharacterClicked(AODWidgetCharacter_Select* Player);
	void CameraAction(AODWidgetCharacter_Select* SelectCharacter);
	void CameraBlend(const FName BlendCam) const;
	UFUNCTION(BlueprintCallable)
	void DefaultCam();
	UFUNCTION(BlueprintCallable)
	void SetAllNotSelected();
	UFUNCTION(BlueprintCallable)
	void InitCam(APlayerController* Controller);
	



private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TArray<AODWidgetCharacter_Select*> SelectCharacters;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TMap<FName,AActor*> CamerasMap;
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BlendTime = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UODWidget_PrimaryLayout> ODPrimaryLayoutWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ClickSound;

	
	
};
