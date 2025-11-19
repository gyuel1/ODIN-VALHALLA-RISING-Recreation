// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODCombatCharacter.h"
#include "Engine/SkeletalMesh.h"
#include "ODMonsterBase.generated.h"

class UODRewardComponent;
class UODMonsterRewardAttributeSet;
class UODWidget_HUDMonsterInfo;
class UODWidget_Target;
class UODWidget_OverHeadMonster;
class UWidgetComponent;
class AODMonsterSpawnActor;
class UODAttributeSet;

UENUM(BlueprintType)
enum class EMonsterStatus : uint8
{
	Idle,
	Walking,
	Running,
	Attacking,
};

UENUM(BlueprintType)
enum class EMonsterEngageType : uint8
{
	Aggressive,
	Non_Aggressive,
};

UENUM(BlueprintType)
enum class EMonsterSpawnContext : uint8
{
	Field	UMETA(DisplayName = "Field"),
	Dungeon UMETA(DisplayName = "Dungeon"),
};

UENUM(BlueprintType)
enum class EMonsterRank : uint8
{
	Grunt	UMETA(DisplayName = "Grunt"),
	Boss	UMETA(DisplayName = "Boss"),
	DungeonBoss	UMETA(DisplayName = "DungeonBoss"),
};


USTRUCT(BlueprintType)
struct FPerceptionInitValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Value")
	float SightRadiusValue = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="Value")
	float LoseSightRadiusValue = 1200.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Value")
	float VisionAngleDegreeValue = 360.f;
};

USTRUCT(BlueprintType)
struct FTargetSystemValue
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly)
	FVector SpawnLocation;
	
	//몬스터 추적 최대 거리
	UPROPERTY(EditDefaultsOnly, Category="Value")
	float MaxTrackingRadius = 3000.f;
};

USTRUCT(BlueprintType)
struct FMonsterMoveSpeed
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Value")
	float MaxWalkSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category="Value")
	float MaxRunSpeed = 300.f;
};


/**
 * 
 */
UCLASS()
class OD_API AODMonsterBase : public AODCombatCharacter
{
	GENERATED_BODY()
public:
	AODMonsterBase();
	
	virtual void BeginPlay() override;
	
	virtual void InitAbilitySystem() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnAutoTargeted() override;
	virtual void OnAutoTargetCleared() override;
	virtual void OnSelectTargeted() override;
	virtual void OnSelectTargetCleared() override;

	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

	
	TObjectPtr<UODWidget_Target> GetTargetWidget() {return TargetWidget;}
	
	FORCEINLINE UODWidget_OverHeadMonster* GetOverHeadWidget() {return OverHeadWidget;}
	FORCEINLINE UODRewardComponent* GetODRewardComponent() {return ODRewardComponent;}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category="Value")
	TObjectPtr<UODRewardComponent> ODRewardComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Value")
	FPerceptionInitValue PerceptionInitValue;

	//몬스터의 속도
	UPROPERTY(EditDefaultsOnly, Category="Value")
	FMonsterMoveSpeed MonsterMoveSpeeds;
	
	//몬스터의 공격 타입
	UPROPERTY(EditDefaultsOnly, Category="Value")
	EMonsterEngageType EngageType = EMonsterEngageType::Non_Aggressive;

	//몬스터의 Spawn 위치
	UPROPERTY(EditDefaultsOnly, Category="Value")
	EMonsterSpawnContext SpawnContext = EMonsterSpawnContext::Field;
	
	//몬스터의 등급
	UPROPERTY(EditDefaultsOnly, Category="Value")
	EMonsterRank MonsterRank = EMonsterRank::Grunt;

	UPROPERTY()
	FTargetSystemValue TargetSystemValue;
	
	UPROPERTY()
	TObjectPtr<AActor> TargetPlayerActor;
	
	UPROPERTY()
	TObjectPtr<AODMonsterSpawnActor> MySpawner;
	
#pragma region DieMeshSwitch
public:
	UFUNCTION()
	void SwitchDeathMeshIfNeed(bool bVisibleDieMesh = false);

	UFUNCTION()
	USkeletalMeshComponent* FindDieMesh();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetVisibleDieMesh(bool bVisible);

protected:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CachedDieMesh;
	
	//Death Skeletal Mesh가 따로 있는 경우
	UPROPERTY(EditAnywhere, Category="Value")
	bool bNeedsDeathMesh = false;

	UPROPERTY(EditDefaultsOnly, Category="Value")
	TSubclassOf<AODMonsterBase> MonsterClass;
	
#pragma endregion
	
#pragma region Getter
public:
	virtual AActor* GetTarget() const override;
	FPerceptionInitValue GetPerceptionInitValue();
	EMonsterEngageType GetEngageType();
	FMonsterMoveSpeed GetMonsterMoveSpeed();
	EMonsterSpawnContext GetMonsterSpawnContext();
	EMonsterRank GetEMonsterRank();
	AODMonsterSpawnActor* GetSpawnActor();
	FTargetSystemValue& GetPatrol();
	USkeletalMeshComponent* GetCachedDieMesh();
	TSubclassOf<AODMonsterBase> GetMonsterDieClass();
	bool GetbNeedsDeathMesh();
#pragma endregion

#pragma region Setter
public:	
	void SetTargetPlayerActor(AActor* TargetActor);	
	void SetSpawner(AODMonsterSpawnActor* Spawner);
#pragma endregion
private:
	void ConfigureOverHeadStatWidget();
	void ConfigureHudMonsterInfo();
	void ConfigureTargetWidget();
	void CreatedHudMonsterInfo();

#pragma region Widgets
	UPROPERTY(EditDefaultsOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_OverHeadMonster> OverHeadWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> TargetWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category="Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_Target> TargetWidget;

	UPROPERTY(Transient)
	TObjectPtr<UODWidget_HUDMonsterInfo> MonsterInfo;
	
#pragma endregion Widgets
};
