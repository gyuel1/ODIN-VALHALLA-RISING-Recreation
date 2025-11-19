// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_DecalSpawn.generated.h"

class AOD_AI_AttackAreaDetector;
/**
 * 
 */

UENUM(BlueprintType)
enum class EDecalPatternType : uint8
{
	SingleInstant		UMETA(DisplayName = "Single (Instant)",		ToolTip = "Notify 시작에 1회 스폰/1회 대미지"),
	RepeatingFixed		UMETA(DisplayName = "Repeating (Fixed)",	ToolTip = "고정 지점에서 주기적 스폰/대미지"),
	RepeatingRandom		UMETA(DisplayName = "Repeating (Random)",	ToolTip = "반경 내 랜덤 좌표들로 지정 횟수 스폰/주기적 대미지"),
};

#pragma region DecalParam Structs
USTRUCT(BlueprintType)
struct FDecalParams_Single
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category ="PatternParams", meta=(ToolTip = "전방 오프셋"))
	float ForwardOffset = 300.f;
};

USTRUCT(BlueprintType)
struct FDecalParams_RepeatingFixed
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category ="PatternParams", meta=(ToolTip = "전방 오프셋"))
	float ForwardOffset = 300.f;

	UPROPERTY(EditDefaultsOnly, Category ="PatternParams", meta=(ToolTip = "장판 지속 시간"))
	float Duration = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="PatternParams", meta=(ToolTip = "지속 딜 주기"))
	float TickInterval = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="PatternParams", meta=(ToolTip = "스폰 할 갯수"))
	int32 NumberSpawnCount = 1;
};

USTRUCT(BlueprintType)
struct FDecalParams_RepeatingRandom
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category ="PatternParams", meta=(ToolTip = "전방 오프셋"))
	float ForwardOffset = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category ="PatternParams", meta=(ToolTip = "장판 지속 시간"))
	float Duration = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="PatternParams", meta=(ToolTip = "지속 딜 주기"))
	float TickInterval = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="PatternParams", meta=(ToolTip = "스폰 할 갯수"))
	int32 NumberSpawnCount = 1;

	UPROPERTY(EditDefaultsOnly, Category="PatternParams", meta=(ToolTip = "랜덤 스폰 될 범위"))
	float RandomRadius = 500.f;
};
#pragma endregion

UCLASS()
class OD_API UANS_DecalSpawn : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Decal")
	TSubclassOf<AOD_AI_AttackAreaDetector> AttackArea;

	UPROPERTY()
	TObjectPtr<AOD_AI_AttackAreaDetector> CachedAttackArea;
	
	UPROPERTY(EditDefaultsOnly, Category="Decal", meta=(ToolTip = "데칼 엑터의 스폰 형태"))
	EDecalPatternType PatternType = EDecalPatternType::SingleInstant;

	UPROPERTY(EditDefaultsOnly, Category="Decal", meta=(EditCondition="PatternType == EDecalPatternType::SingleInstant", EditConditionHides, ToolTip = "데칼 패턴 파라미터들"))
	FDecalParams_Single SingleParams;

	UPROPERTY(EditDefaultsOnly, Category="Decal", meta=(EditCondition="PatternType == EDecalPatternType::RepeatingFixed", EditConditionHides, ToolTip = "데칼 패턴 파라미터들"))
	FDecalParams_RepeatingFixed RepeatFixedParams;

	UPROPERTY(EditDefaultsOnly, Category="Decal", meta=(EditCondition="PatternType == EDecalPatternType::RepeatingRandom", EditConditionHides, ToolTip = "데칼 패턴 파라미터들"))
	FDecalParams_RepeatingRandom RepeatingRandomParams;
	
public:
	UFUNCTION()
	void SpawnAttackAreaAt(AActor* OwnerActor, const FVector& SpawnLocation, const FRotator& SpawnRotation = FRotator(0.f,0.f,0.f));
	
#pragma region PatternSingleInstant
public:
	UFUNCTION()
	void PatternSingleInstant(USkeletalMeshComponent* MeshComp);

	UFUNCTION()
	FVector FixToGround(UWorld* World, const FVector& Start, float DownDistance = 5000.f, float UpOffset = 50.f);
	
#pragma endregion

#pragma region PatternRepeationRandom
public:
	UFUNCTION()
	void PatternRepeatingRandom(USkeletalMeshComponent* MeshComp);

	UFUNCTION()
	FVector CalSpawnLocation(UWorld* MeshComp, const FVector& Origin, float Radius);
	
#pragma endregion
	
};
