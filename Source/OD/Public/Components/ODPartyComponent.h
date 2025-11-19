// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODPlayer.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ODPartyComponent.generated.h"


class UODPartyInfo;

USTRUCT()
struct FODPartyPacket
{
	GENERATED_BODY()

	UPROPERTY()
	UODPartyInfo* PartyInfo;

	FODPartyPacket()
	{
		PartyInfo = nullptr;
	}
	FODPartyPacket(UODPartyInfo* InPartyInfo)
	{
		PartyInfo = InPartyInfo;
	}
	
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OD_API UODPartyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UODPartyComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UODPartyInfo* GetParty() const { return Party; };

	//===================== UI 호출 함수 ======================//
	
	//파티 만들기 (리더옵션)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void CreateParty();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void CreateParty_Server();
	
	//파티 해체 (리더 옵션)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void RemovePartyLeaderOption();

	//파티 대상을 파티에 추가(리더 옵션)
	 UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	 void AddMember(AODPlayer* InPlayer);
	//파티 대상을 파티에 추가(리더 옵션)
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddMember_Server(AODPlayer* InPlayer);
	
	//파티 탈퇴 (리더, 멤버 옵션) (리더가 탈퇴하면 누군가가 리더 계승)
	UFUNCTION(NetMulticast, Reliable,BlueprintCallable)
	void LeaveParty();
	
	//강퇴 (리더 옵션) (목록에서 대상을 선택후 인자로 집어넣기)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void RemoveMember(AODPlayer* InPlayer);
	
	
	//===================== 조건 검사 ======================//

	//파티중인가?
	bool HasParty() const;

	//권한검사
	bool IsPartyLeader() const;

	//===================== 내부 함수 ======================//
	UFUNCTION(NetMulticast, Reliable)
	void SetParty(UODPartyInfo* InParty);

	UFUNCTION(NetMulticast, Reliable)
	void OnRepPartyValue(UODPartyInfo* InParty);

	UFUNCTION(NetMulticast, Reliable)
	void RemovePartyMemberOption();

	//===================================================//
	
	UFUNCTION(Client, Reliable)
	void OnRepPartyPacket(const FODPartyPacket PartyPacket);
	
	UFUNCTION(Server, Reliable,BlueprintCallable)
	void ConnectDungeon();

	
	
	void ConnectDungeon_Member();
	
	UFUNCTION(NetMulticast, Reliable)
	void RemovePartyMemberOption_Client();
	
	UFUNCTION(BlueprintCallable)
	void LogPartyList();

	UFUNCTION(Server,Reliable, BlueprintCallable)
	void LogPartyList_Server();

	UFUNCTION()
	void OnRep_Party();

private:
	UPROPERTY(ReplicatedUsing = OnRep_Party)
	TObjectPtr<UODPartyInfo> Party;

	FTimerHandle LevelStreamingTimerHandle;
};

