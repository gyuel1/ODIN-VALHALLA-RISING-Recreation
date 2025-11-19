// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODCharacter.h"
#include "ODWidgetCharacter.generated.h"

/**
 * 
 */
UCLASS()
class OD_API AODWidgetCharacter : public AODCharacter
{
	GENERATED_BODY()

public:
	AODWidgetCharacter();

protected:
	virtual void BeginPlay() override;
	

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> BodySkeletalMesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> HairSkeletalMesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> FaceSkeletalMesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;
};
