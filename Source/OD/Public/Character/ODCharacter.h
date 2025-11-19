// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Info/ODObjectInfo.h"
#include "Interface/ODRepGraphInterface.h"
#include "ODCharacter.generated.h"

UENUM()
enum class EODCharacterType :uint8
{
	None,
	Player,

	//monster
	FieldGruntMonster,
	FieldBossMonster,
	DungeonGruntMonster,
	DungeonBossMonster,
	
	//npc
	GruntNPC,
	QuestNPC,
	StoreNPC
	
	
};

UCLASS()
class OD_API AODCharacter : public ACharacter, public IODRepGraphInterface
{
	GENERATED_BODY()

public:
	AODCharacter();

	FORCEINLINE FText GetDisplayName() const {return DisplayName;}
	FORCEINLINE const EODCharacterType& GetODCharacterTyp() const {return ODCharacterType;}

	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ODCharacterInfo", meta = (AllowPrivateAccess = "true"))
	FText DisplayName;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ODCharacterInfo", meta = (AllowPrivateAccess = "true"))
	EODCharacterType ODCharacterType;

	UPROPERTY()
	int32 RepGraphCount;

	
};
