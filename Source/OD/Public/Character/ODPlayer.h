// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Character/ODCombatCharacter.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Structs/StatsDataStruct.h"
#include "ODPlayer.generated.h"

class UImage;
class UODPartyComponent;
class UODSkillComponent;
class UWidgetComponent;
class UODGameplayAbility;
class USphereComponent;
class UODTargetComponent;
class UODCameraControllerComponent;
class UODTargetingAndAttackComponent;
class UODActiveSkillDA;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoHuntChanged, bool, bIsAutoHunt);

UCLASS()
class OD_API AODPlayer : public AODCombatCharacter
{
	GENERATED_BODY()
public:
	AODPlayer();
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Jump() override;
	
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//GAS Initialized
	virtual void InitAbilitySystem() override;

	UFUNCTION(Client, Reliable)
	void InitAbilitySystem_Client();
	//네트워크 복제
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	void ConfigureOverHeadStatWidget();

	UFUNCTION()
	void OnRep_PawnPlayerName();

	// 서버에서 호출하여 이름을 설정하고 복제를 시작하는 함수
	void SetPawnPlayerName(const FString& NewName); 
	
	
	//어빌리티 테스트용 시작
private:

	UFUNCTION(BlueprintCallable)
	void TestSkillActiveAbility();

	UFUNCTION(BlueprintCallable)
	void TestSkillActiveAbilityByTag(FGameplayTag InTag);

	UFUNCTION(BlueprintCallable)
	void TestAddSlotAbilityByTag(int32 InSlotIndex,FGameplayTag InTag);

	UFUNCTION(BlueprintCallable)
	void TestAddSlotAbility();

	UFUNCTION(BlueprintCallable)
	void TestRemoveSlotAbility();

	//어빌리티 테스트용 끝


public:
	FORCEINLINE UODSkillComponent* GetODSkillComponent() const {return SkillComponent;};

	FORCEINLINE UODPartyComponent* GetODPartyComponent() const {return PartyComponent;};
	
	FORCEINLINE USphereComponent* GetAutoTargetCollision() const { return AutoTargetCollision; }
	
	FORCEINLINE UWidgetComponent* GetOverHeadWidgetComponent() const {return OverHeadWidgetComponent;}

	FORCEINLINE UODTargetingAndAttackComponent* GetTargetingAndAttackComponent() const {return TargetingAndAttackComponent;}

	FORCEINLINE bool GetAutoHuntToggledByUser() const {return bAutoHuntToggledByUser;}

	FORCEINLINE void SetAutoHuntToggledByUser(bool bIsAutoHunt) {bAutoHuntToggledByUser = bIsAutoHunt;}

	FORCEINLINE FString GetPlayerName() const {return ReplicatedPawnName;}
	

	UFUNCTION(BlueprintCallable)
	virtual AActor* GetTarget() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UODCameraControllerComponent> CameraControllerComponent;
	
	FORCEINLINE const FLevelUpStats& GetLevelUpStats() const {return LevelUpStats;};

	
private:
	
	//TargetComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODTargetComponent> TargetComponent;
	//TargetingAndAttackComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODTargetingAndAttackComponent> TargetingAndAttackComponent;

	//SkillComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODSkillComponent> SkillComponent;

	//PartyComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODPartyComponent> PartyComponent;

	//LevelUp Data
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ODCharacterInfo",meta = (AllowPrivateAccess = true))
	FLevelUpStats LevelUpStats;

	//AutoTargetCollision
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> AutoTargetCollision;

	// OverHeadWidgetComponent
	UPROPERTY(VisibleAnywhere, Category= "UI")
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;



public:
	UFUNCTION(Server, Reliable)
	void ServerMouseMoveToLocation(const FVector& Destination);
	UFUNCTION(Server, Reliable)
	void ServerSetMouseMovingClick(bool MouseMoving);
	UFUNCTION(Server, Reliable)
	void ServerMouseStopMovement();
	UFUNCTION(Server, Reliable)
	void ServerMoveToTarget(AActor* Target);
	UFUNCTION(Server, Reliable)
	void Server_EndCombat();
	UFUNCTION()
	void OnRep_InCombat();
	UPROPERTY(ReplicatedUsing=OnRep_InCombat, VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	bool InCombat = false;

	//전투 상태 판정
	UFUNCTION(Server, Reliable)
	void MarkCombatStart();
	//자동사냥 켜고 끄기
	bool IsAutoHunt = false;
	bool WasAutoHuntingBeforeMove = false;

	void StartAutoHunt();
	void StopAutoHunt();
	

	FOnAutoHuntChanged OnAutoHuntChanged;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext>  GameplayInputMappingContext;
	
	//카메라
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ZoomInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;
		
	//이동
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;
	//점프
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpInputAction;
	//마우스 이동
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SetDestinationClickAction;
	//타겟팅
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> TabTargetInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RTargetInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RTargetCancelInputAction;
	//타겟팅 공격
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackTargetInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AutoHuntInputAction ;
	//스킬사용
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>  Skill1InputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>  Skill2InputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>  Skill3InputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>  Skill4InputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>  Skill5InputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>  Skill6InputAction;
	//파티
	UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction>  PartyInputAction;
	
		
	//카메라
	void HandleZoom(const FInputActionValue& InputActionValue);
	void HandleLooKInput(const FInputActionValue& InputActionValue );
	//이동 키보드
	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleMoveInput_Completed(const FInputActionValue& InputActionValue);
	FVector GetMoveFWdDir()const;
	//이동 마우스
	void OnMouseButtonPressed(const FInputActionValue& InputActionValue);
	void OnMouseButtonReleased(const FInputActionValue& InputActionValue);
	//타겟팅
	void HandleTabTargetButton(const FInputActionValue& InputActionValue);
	void HandleRTargetButton(const FInputActionValue& InputActionValue);
	void RTargetCancel(const FInputActionValue& InputActionValue);
	//타겟팅 공격
	void HandleAttackTargetButton(const FInputActionValue& InputActionValue);
	void HandleAutoHunt(const FInputActionValue& InputActionValue);
	//스킬사용
	void Skill1Input(const FInputActionValue& InputActionValue);
	void Skill2Input(const FInputActionValue& InputActionValue);
	void Skill3Input(const FInputActionValue& InputActionValue);
	void Skill4Input(const FInputActionValue& InputActionValue);
	void Skill5Input(const FInputActionValue& InputActionValue);
	void Skill6Input(const FInputActionValue& InputActionValue);
	//파티창
	void PartyUIInput(const FInputActionValue& InputActionValue);
	//void CheckMouseMoveCompletion();
	
	//자동사냥 복귀여부
	bool bUserExplicitlyCanceledAutoHunt = false;
	bool bAutoHuntToggledByUser = false;
	FTimerHandle MouseMoveCheckTimerHandle;
	
	//카메라 회전중 마우스 막기
	bool IsKeyboardMoveActive() const;
	//카메라 회전중 마우스 이동 막기
	bool bIsUsingKeyboardForMove = false;
	
	
	// OverHeadWidget
	FTimerHandle OHSVisibilityUpdateTimerHandle;
	void UpdateOHSVisibility();
	//OHS 체크시간
	UPROPERTY(EditDefaultsOnly, Category= "UI")
	float OHSCheckTime = 1.0f;

	//로컬 플레이어와 가시거리 기준값
	UPROPERTY(EditDefaultsOnly, Category= "UI")
	float OHSVisibilityRange = 1000000.f;


	// 이름
	UPROPERTY(ReplicatedUsing = OnRep_PawnPlayerName) 
	FString ReplicatedPawnName;
	//스킬사용
public:
	UFUNCTION()
	void ProcessSkillInputBySlotIndex(uint32 SkillIndex);
	
	bool TryActivateSkillBySlotIndex(uint32 SlotIndex);
	
	


	
	
};
