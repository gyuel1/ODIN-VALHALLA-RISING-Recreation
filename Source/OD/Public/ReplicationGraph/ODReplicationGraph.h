// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "Interface/ODRepGraphInterface.h"
#include "ODReplicationGraph.generated.h"

class AODPlayer;
class AGameplayDebuggerCategoryReplicator;
/**
 * 
 */
UCLASS()
class OD_API UODReplicationGraph : public UReplicationGraph
{
	GENERATED_BODY()

public:
	UODReplicationGraph();

	
	virtual void InitForNetDriver(UNetDriver* InNetDriver) override;
	
	virtual void InitGlobalGraphNodes() override;
	virtual void InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager) override;

	virtual void InitGlobalActorClassSettings() override;
	virtual void RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo,
	                                         FGlobalActorReplicationInfo& GlobalInfo) override;
	virtual void RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo) override;

	
	void CreatePlayerNode(APlayerController* NewPlayer);
	void RemovePlayerNode(APlayerController* Player);

	
	void AddRepGraphSpatialNode(const int32 InCount,UNetReplicationGraphConnection* InConnectionManager, AODPlayer* InPlayer);
	void RemoveGraphSpatialNode(const int32 InCount,UNetReplicationGraphConnection* InConnectionManager, AODPlayer* InPlayer);


	void ChangeChannelActor(AActor* InChangeActor,const int32 InNewChannel);
	int32 OpenChannelPlayer(AODPlayer* InPlayer, const int32 InNewChannel,const AActor* SpawnPoint = nullptr);
	void CloseChannelPlayer(AODPlayer* InPlayer, const int32 InOldChannel);
	void ChangeChannelPlayer(AODPlayer* InPlayer, const int32 InNewChannel,const AActor* SpawnPoint = nullptr);

	

private:

	int32 DungeonCount;
	
	UPROPERTY()
	UReplicationGraphNode_ActorList* NonSpatializedNode = nullptr;

	UPROPERTY()
	UReplicationGraphNode_GridSpatialization2D* MonsterGrid = nullptr;
	

	UPROPERTY()
	TMap<APlayerController*, UReplicationGraphNode_AlwaysRelevant_ForConnection*> PCNodeMap;

	UPROPERTY()
	TMap<int32, UReplicationGraphNode_ActorList*> SpatializedNodeMap;

	UPROPERTY()
	TArray<AGameplayDebuggerCategoryReplicator*> Debuggers;
	FTSTicker::FDelegateHandle DebuggersTickHandle;

	void StartDebuggersTick(AGameplayDebuggerCategoryReplicator* InDebugger);
	void StopDebuggersTick();

	bool TickDebuggers(float DeltaTime);

	
	
};





		