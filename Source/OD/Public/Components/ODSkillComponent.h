// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "PaperSprite.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "ODSkillComponent.generated.h"


class AODPlayer;
class UODSkillDA;
class UODActiveSkillDA;
class UODPassiveSkillDA;


USTRUCT(BlueprintType)
struct FSkillCooldownDuration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentCooldown;

	FSkillCooldownDuration()
	{
		TotalCooldown = 0.f;
		CurrentCooldown = 0.f;
	}

	FSkillCooldownDuration(const float& InTotalCooldown, const float& InCurrentCooldown)
	{
		TotalCooldown = InTotalCooldown;
		CurrentCooldown = InCurrentCooldown;
	}

	bool IsValid() const
	{
		return TotalCooldown > 0.f && CurrentCooldown > 0.f;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetCooldownDele,FSkillCooldownDuration,killCooldownDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillActivateDele);

USTRUCT(BlueprintType)
struct FSkillSlot
{
	GENERATED_BODY()

	//스킬태그
	UPROPERTY()
	FGameplayTag SkillTag;
	
	//스킬 아이콘
	UPROPERTY()
	TObjectPtr<UPaperSprite> SkillIcon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	//쿨다운 관제자
	UPROPERTY()
	FActiveGameplayEffectHandle CooldownHandle;
	//쿨다운 타이머 핸들
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	//깡쿨타임
	UPROPERTY()
	FSkillCooldownDuration CooldownDuration;

	//스킬슬롯 바인드(스킬 쿨이 돌때 실시간으로 스킬 쿨타임을 인자로 넘겨줌)
	UPROPERTY(BlueprintAssignable)
	FWidgetCooldownDele WidgetCooldownDele;
	
	FSkillActivateDele WidgetActivateDele;
	

	FSkillSlot()
	{
		SkillTag = FGameplayTag();
		SkillIcon = nullptr;
		CooldownHandle = FActiveGameplayEffectHandle(INDEX_NONE);
	}

	FSkillSlot(const FGameplayTag& InSkillTag, UPaperSprite* InSkillIcon)
	{
		SkillTag = InSkillTag;
		SkillIcon = InSkillIcon;
		CooldownHandle = FActiveGameplayEffectHandle(INDEX_NONE);
	}
	

	//스킬을 슬롯에 등록
	void AssignSlot(UODActiveSkillDA* InActiveDA,UAbilitySystemComponent* InASC);

	//스킬을 슬롯에서 해제
	void ClearSlot();

	//쿨타임 등록 (해당 스킬 어빌리티 발동시 호출)
	void OnSkillCooldown_Server(const FActiveGameplayEffectHandle InCooldownHandle);
	void OnSkillCooldown_Client(const FGameplayTagContainer InTagContainer);

	//스킬 쿨타임인지
	bool IsCooldown();

	//쿨타운 종료
	void ClearCooldown();
	
	//GetCooldown (Total, Current 를 담고있는 구조체 리턴)
	FSkillCooldownDuration GetCooldownDuration();

	//슬롯에 스킬이 세팅이 되었는지
	FORCEINLINE bool IsValid() const
	{
		return SkillTag.IsValid();
	}
	
};



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OD_API UODSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UODSkillComponent();

	virtual void InitializeComponent() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//스킬 목록 Get
	TArray<TObjectPtr<UODPassiveSkillDA>>& GetPassiveSkills();
	TArray<TObjectPtr<UODActiveSkillDA>>& GetActiveSkills();

	TArray<FSkillSlot>& GetSkillSlots();

	//태그로 스킬 슬롯 찾기
	FSkillSlot* FindSkillSlotByTag(const FGameplayTag& InTag);

	//스킬 컨테이너에서 스킬 찾기 return == UODSkillDA -> 패시브인지 액티브인지 알필요가 없을 경우 사용
	UODSkillDA* FindSkillDAByTag(const FGameplayTag& InTag);

	//스킬 컨테이너에서 스킬 찾기 return == (template) -> 패시브인지 액티브인지 판단해야하면 사용
	template <typename T>
	T* FindSkillDAByTag(const FGameplayTag& InTag);

	//스킬이 배워져 있는가?
	bool IsUnlockSkill(const FGameplayTag& InTag);

	//스킬 배우기 (배우기에 성공했다면 return true)
	UFUNCTION(Server, Reliable)
	void UnlockSkill(const FGameplayTag& InTag);

	//스킬을 슬롯에 등록
	
	void AddSkillToSlot(const uint32& InIndex, const FGameplayTag& InTag);
	void AddSkillToSlot_Internal(const uint32& InIndex, const FGameplayTag& InTag);
	
	UFUNCTION(Server, Reliable)
	void AddSkillToSlot_Server(const uint32 InIndex, const FGameplayTag InTag);
	
	
	//스킬을 슬롯에서 해제
	void RemoveSkillFromSlot(const uint32& InIndex);
	void RemoveSkillFromSlot_Internal(const uint32& InIndex);
	
	UFUNCTION(Server, Reliable)
	void RemoveSkillFromSlot_Server(const uint32 InIndex);
	
	
	//스킬 사용? (스킬을 스킬 큐에 등록?)
	bool TryActiveSkill(const uint32& SlotIndex);

	UFUNCTION(Client, Reliable)
	void OnSkillSlotAnimation(const int32 Inindex);

	
	


protected:
	virtual void BeginPlay() override;

private:
	//패시브 스킬 목록
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="SkillBook", meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UODPassiveSkillDA>> PassiveSkillArray;

	//액티브 스킬 목록
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="SkillBook", meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UODActiveSkillDA>> ActiveSkillArray;

	//스킬 슬롯
	UPROPERTY(Replicated)
	TArray<FSkillSlot> SkillSlotArray;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC;

	UPROPERTY()
	FTimerHandle InitTimerHandle;
};

template <typename T>
T* UODSkillComponent::FindSkillDAByTag(const FGameplayTag& InTag)
{
	UODSkillDA* SkillDA = FindSkillDAByTag(InTag);

	if (T* CastSkillDA = Cast<T>(SkillDA))
		return CastSkillDA;

	return nullptr;
}
