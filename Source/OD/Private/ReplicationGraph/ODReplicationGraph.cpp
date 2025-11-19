// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicationGraph/ODReplicationGraph.h"

#include "GameplayDebuggerCategoryReplicator.h"
#include "GameplayDebuggerPlayerManager.h"
#include "ReplicationGraph.h"
#include "ReplicationGraphTypes.h"
#include "AI/Spawning/ODMonsterSpawnActor.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Debug/ODLogChannels.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Interface/ODRepGraphInterface.h"
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/ODDungeonInstance.h"
#include "Object/ODObjectBase.h"




UODReplicationGraph::UODReplicationGraph()
{
	DungeonCount= 1;
}

void UODReplicationGraph::InitForNetDriver(UNetDriver* InNetDriver)
{
	Super::InitForNetDriver(InNetDriver);
	
}

void UODReplicationGraph::InitGlobalGraphNodes()
{
	Super::InitGlobalGraphNodes();

	//전역 복제
	NonSpatializedNode = CreateNewNode<UReplicationGraphNode_ActorList>();
	AddGlobalGraphNode(NonSpatializedNode);

	UReplicationGraphNode_ActorList* DefaultNode = CreateNewNode<UReplicationGraphNode_ActorList>();
	SpatializedNodeMap.Add(0, DefaultNode);

	//몬스터 거리로 복제
	MonsterGrid = CreateNewNode<UReplicationGraphNode_GridSpatialization2D>();
	MonsterGrid->CellSize = 240000.f;             
	MonsterGrid->SpatialBias = FVector2D(0.f, 0.f);  
	
}

void UODReplicationGraph::InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager)
{
	Super::InitConnectionGraphNodes(ConnectionManager);

	AddConnectionGraphNode(NonSpatializedNode, ConnectionManager);
	AddConnectionGraphNode(MonsterGrid, ConnectionManager);

	//기본 복제 0번채널
	if (SpatializedNodeMap.Contains(0))
	{
		AddConnectionGraphNode(*SpatializedNodeMap.Find(0), ConnectionManager);
	}
}

void UODReplicationGraph::InitGlobalActorClassSettings()
{
	Super::InitGlobalActorClassSettings();
}

void UODReplicationGraph::RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo,
                                                      FGlobalActorReplicationInfo& GlobalInfo)
{
	
	AActor* A = ActorInfo.Actor;
	if (!A || !A->GetIsReplicated() || A->IsActorBeingDestroyed() || !A->GetWorld())
		return;

	//전역 복제
	if (A->IsA<APlayerState>() || A->IsA<AGameState>() || A->IsA<AGameModeBase>() || A->IsA<AODDungeonInstanceManager>() || A->IsA(AODMonsterSpawnActor::StaticClass()))
	{
		NonSpatializedNode->NotifyAddNetworkActor(ActorInfo);
		return;
	}
	//플레이어 컨트롤러
	if (A->IsA<APlayerController>())
	{
		return;
	}

	//게임플레이 디버거
	if (A->IsA<AGameplayDebuggerCategoryReplicator>())
	{
		AGameplayDebuggerCategoryReplicator* Debugger = Cast<AGameplayDebuggerCategoryReplicator>(A);
		StartDebuggersTick(Debugger);
		return;
	}

	//몬스터는 거리별 관리
	if (A->IsA(AODMonsterBase::StaticClass()) ||  A->IsA(AODObjectBase::StaticClass()) || A->IsA(AODMonsterSpawnActor::StaticClass()))
	{
		// 클래스 기본거리 말고 이 개체만 5,000uu로
		GlobalInfo.Settings.SetCullDistanceSquared(FMath::Square(4000.f));
		MonsterGrid->AddActor_Dormancy(ActorInfo,GlobalInfo);
		return;
	}

	if (A->IsA(AODDungeonInstance::StaticClass()))
	{
		if (IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(A))
		{
			
			UReplicationGraphNode_ActorList* MakeNode = CreateNewNode<UReplicationGraphNode_ActorList>();
			MakeNode->NotifyAddNetworkActor(ActorInfo);
			SpatializedNodeMap.Add(DungeonCount, MakeNode);
			DungeonCount++;
			return;
		}
	}

 
	//RepCount 검사
	int32 Count = 0;
	if (IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(A))
	{
		Count = RGI->GetRepGraphCount();
	}
	else
	{
		NonSpatializedNode->NotifyAddNetworkActor(ActorInfo);
		return;
	}

	//조건 복제
	UReplicationGraphNode_ActorList* FindGrid = nullptr;
	if (SpatializedNodeMap.Contains(Count))
	{
		FindGrid = *SpatializedNodeMap.Find(Count);
	}

	if (FindGrid)
	{
		FindGrid->NotifyAddNetworkActor(ActorInfo);
	}
	else
	{
		FindGrid = CreateNewNode<
			UReplicationGraphNode_ActorList>();

		SpatializedNodeMap.Add(Count, FindGrid);
		FindGrid->NotifyAddNetworkActor(ActorInfo);
	}

	//NonSpatializedNode->NotifyAddNetworkActor(ActorInfo);
}

void UODReplicationGraph::RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo)
{
	AActor* A = ActorInfo.Actor;
	if (!A) return;
	

	//전역 복제 제거
	if (A->IsA<APlayerState>() || A->IsA<AGameState>() || A->IsA<AGameModeBase>() ||  A->IsA<AODDungeonInstanceManager>() || A->IsA(AODMonsterSpawnActor::StaticClass()))
	{
		NonSpatializedNode->NotifyRemoveNetworkActor(ActorInfo);
		return;
	}
	//플레이어 컨트롤러
	if (A->IsA<APlayerController>())
	{
		return;
	}
	
	//게임 플레이 디버거
	if (A->IsA<AGameplayDebuggerCategoryReplicator>())
	{
		AGameplayDebuggerCategoryReplicator* Debugger = Cast<AGameplayDebuggerCategoryReplicator>(A);
	
		if (APlayerController* PC = Cast<APlayerController>(Debugger->GetOwner()))
		{
			if (PCNodeMap.Contains(PC))
			{
				UReplicationGraphNode_AlwaysRelevant_ForConnection* Node = *PCNodeMap.Find(PC);
				Node->NotifyRemoveNetworkActor(ActorInfo);
			}
		}
		return;
	}

	//몬스터는 거리별 관리
	if (A->IsA(AODMonsterBase::StaticClass()) || A->IsA(AODObjectBase::StaticClass()))
	{
		MonsterGrid->RemoveActor_Dormancy(ActorInfo);
		return;
	}

	if (A->IsA(AODDungeonInstance::StaticClass()))
	{
		if (IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(A))
		{
			if (SpatializedNodeMap.Contains(RGI->GetRepGraphCount()))
			{
				UReplicationGraphNode_ActorList* MakeNode = *SpatializedNodeMap.Find(RGI->GetRepGraphCount());
				MakeNode->NotifyRemoveNetworkActor(ActorInfo);
			}
		}
	}

	int32 Count = 0;
	if (IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(A))
	{
		Count = RGI->GetRepGraphCount();
	}
	else
	{
		NonSpatializedNode->NotifyRemoveNetworkActor(ActorInfo);
		return;
	}

	if (SpatializedNodeMap.Contains(Count))
	{
		if (UReplicationGraphNode_ActorList* Grid = *SpatializedNodeMap.Find(Count))
		{
			Grid->NotifyRemoveNetworkActor(ActorInfo);
		}
	}

	//NonSpatializedNode->NotifyRemoveNetworkActor(ActorInfo);
}

void UODReplicationGraph::CreatePlayerNode(APlayerController* NewPlayer)
{
	
	UReplicationGraphNode_AlwaysRelevant_ForConnection* PCNode = nullptr;
	for (UNetReplicationGraphConnection* Conn : Connections)
	{
		if (Conn && Conn->NetConnection->PlayerController == NewPlayer)
		{
			if (PCNodeMap.Contains(NewPlayer))
			{
				PCNode = *PCNodeMap.Find(NewPlayer);
			}
			else
			{
				PCNode = CreateNewNode<UReplicationGraphNode_AlwaysRelevant_ForConnection>();
				PCNodeMap.Add(NewPlayer, PCNode);
			}

			const FNewReplicatedActorInfo PlayerControllerInfo(NewPlayer);
			PCNode->NotifyAddNetworkActor(PlayerControllerInfo);
			AddConnectionGraphNode(PCNode, Conn);
			break;
		}
	}
}

void UODReplicationGraph::RemovePlayerNode(APlayerController* Player)
{
	for (UNetReplicationGraphConnection* Conn : Connections)
	{
		if (Conn && Conn->NetConnection->PlayerController == Player)
		{
			if (PCNodeMap.Contains(Player))
			{
				auto* PCNode = *PCNodeMap.Find(Player);
				const FNewReplicatedActorInfo Info(Player);
				PCNode->NotifyRemoveNetworkActor(Info);
				break;
			}
		}
	}
}

void UODReplicationGraph::AddRepGraphSpatialNode(const int32 InCount, UNetReplicationGraphConnection* InConnectionManager, AODPlayer* InPlayer)
{
	if (!InConnectionManager || !SpatializedNodeMap.Contains(InCount) || !InPlayer)
		return;
	
	if (UReplicationGraphNode_ActorList* SpatializedNode = *SpatializedNodeMap.Find(InCount))
	{
		AddConnectionGraphNode(SpatializedNode,InConnectionManager);
	}
}

void UODReplicationGraph::RemoveGraphSpatialNode(const int32 InCount,
	UNetReplicationGraphConnection* InConnectionManager , AODPlayer* InPlayer)
{
	if (!InConnectionManager || !SpatializedNodeMap.Contains(InCount) || !InPlayer)
		return;

	if (UReplicationGraphNode_ActorList* SpatializedNode = *SpatializedNodeMap.Find(InCount))
	{
		RemoveConnectionGraphNode(SpatializedNode,InConnectionManager);
	}
}

void UODReplicationGraph::ChangeChannelActor(AActor* InChangeActor, const int32 InNewChannel)
{
	
	IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(InChangeActor);
	if (!RGI) return;

	const int32 NewChannel = InNewChannel;
	const int32 OldChannel = RGI->GetRepGraphCount();

	if (!SpatializedNodeMap.Contains(NewChannel))
		return;

	UReplicationGraphNode_ActorList* OldNode = *SpatializedNodeMap.Find(OldChannel);
	if (!OldNode)
		return;

	UReplicationGraphNode_ActorList* NewNode = *SpatializedNodeMap.Find(NewChannel);
	if (!NewNode)
		return;
	
}

int32 UODReplicationGraph::OpenChannelPlayer(AODPlayer* InPlayer, const int32 InNewChannel,const AActor* SpawnPoint)
{
	if (!InPlayer || InNewChannel < 0)
		return -1;

	IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(InPlayer);
	if (!RGI) return -1;

	//현재채널, 새로운 채널 캐싱
	const int32 NewChannel = InNewChannel;
	const int32 OldChannel = RGI->GetRepGraphCount();
	
	//플레이어 커넥션 찾기
	APlayerController* PC = InPlayer->GetController<APlayerController>();
	if (!PC) return -1;

	UNetReplicationGraphConnection* PlayerConnection = nullptr;
	for (UNetReplicationGraphConnection* Conn : Connections)
	{
		if (Conn && Conn->NetConnection->PlayerController == PC)
		{
			PlayerConnection = Conn;
			break;
		}
	}
	if (!PlayerConnection)
		return -1;

	//플레이어 커넥션에 새로운 채널 추가
	if (!SpatializedNodeMap.Contains(NewChannel))
		return -1;
	
	UReplicationGraphNode_ActorList* NewNode = *SpatializedNodeMap.Find(NewChannel);
	AddConnectionGraphNode(NewNode, PlayerConnection);
	RGI->SetRepGraphCount(NewChannel);
	return OldChannel;
}

void UODReplicationGraph::CloseChannelPlayer(AODPlayer* InPlayer, const int32 InOldChannel)
{
	//플레이어 커넥션 찾기
	APlayerController* PC = InPlayer->GetController<APlayerController>();
	if (!PC) return;

	UNetReplicationGraphConnection* PlayerConnection = nullptr;
	for (UNetReplicationGraphConnection* Conn : Connections)
	{
		if (Conn && Conn->NetConnection->PlayerController == PC)
		{
			PlayerConnection = Conn;
			break;
		}
	}
	if (!PlayerConnection)
		return;
	
	//플레이어 커넥션에 이전 채널 제거
	if (!SpatializedNodeMap.Contains(InOldChannel))
		return;
	
	UReplicationGraphNode_ActorList* OldNode = *SpatializedNodeMap.Find(InOldChannel);
	RemoveConnectionGraphNode(OldNode, PlayerConnection);
}

void UODReplicationGraph::ChangeChannelPlayer(AODPlayer* InPlayer, const int32 InNewChannel,const AActor* SpawnPoint)
{
	if (!InPlayer || InNewChannel < 0)
		return;

	IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(InPlayer);
	if (!RGI) return;

	//현재채널, 새로운 채널 캐싱
	const int32 NewChannel = InNewChannel;
	const int32 OldChannel = RGI->GetRepGraphCount();
	
	//플레이어 커넥션 찾기
	APlayerController* PC = InPlayer->GetController<APlayerController>();
	if (!PC) return;

	UNetReplicationGraphConnection* PlayerConnection = nullptr;
	for (UNetReplicationGraphConnection* Conn : Connections)
	{
		if (Conn && Conn->NetConnection->PlayerController == PC)
		{
			PlayerConnection = Conn;
			break;
		}
	}
	if (!PlayerConnection)
		return;

	//플레이어 커넥션에 새로운 채널 추가
	if (!SpatializedNodeMap.Contains(NewChannel))
		return;
	
	UReplicationGraphNode_ActorList* NewNode = *SpatializedNodeMap.Find(NewChannel);
	AddConnectionGraphNode(NewNode, PlayerConnection);
	RGI->SetRepGraphCount(NewChannel);

	//플레이어 캐릭터를 새로운 채널 맵으로 이동
	if (SpawnPoint)
	{
		InPlayer->SetActorLocation(SpawnPoint->GetActorLocation());
	}
	
	//플레이어 커넥션에 이전 채널 제거
	if (!SpatializedNodeMap.Contains(OldChannel))
		return;
	
	UReplicationGraphNode_ActorList* OldNode = *SpatializedNodeMap.Find(OldChannel);
	RemoveConnectionGraphNode(OldNode, PlayerConnection);
}


void UODReplicationGraph::StartDebuggersTick(AGameplayDebuggerCategoryReplicator* InDebugger)
{
	Debuggers.Add(InDebugger);
	if (!DebuggersTickHandle.IsValid())
	{
		DebuggersTickHandle = FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateUObject(this, &UODReplicationGraph::TickDebuggers), 0.0f);
	}
}

void UODReplicationGraph::StopDebuggersTick()
{
	if (DebuggersTickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(DebuggersTickHandle);
		DebuggersTickHandle.Reset();
	}
}

bool UODReplicationGraph::TickDebuggers(float DeltaTime)
{
	if (Debuggers.IsEmpty())
	{
		StopDebuggersTick();
		return false;
	}
	UE_LOG(LogODConnct,Warning,TEXT("%d"),Debuggers.Num());

	TArray<AGameplayDebuggerCategoryReplicator*> RemoveDebuggers;
	for (AGameplayDebuggerCategoryReplicator* Debugger : Debuggers)
	{
		if (!Debugger)
			continue;


		for (UNetReplicationGraphConnection* Conn : Connections)
		{
			if (Conn && Conn->NetConnection->PlayerController)
			{
				APlayerController* PC = Conn->NetConnection->PlayerController;
				if (APlayerController* DebuggerPC = Debugger->GetReplicationOwner())
				{
					if (DebuggerPC != PC)
						continue;

					if (PCNodeMap.Contains(PC))
					{
						UReplicationGraphNode_AlwaysRelevant_ForConnection* Node = *PCNodeMap.Find(PC);
						FNewReplicatedActorInfo DebuggerInfo(Debugger);
						Node->NotifyAddNetworkActor(DebuggerInfo);
						RemoveDebuggers.Add(Debugger);
						break;
					}
				}
			}
		}
	}
		for (AGameplayDebuggerCategoryReplicator* RemoveConn :RemoveDebuggers)
		{
			Debuggers.Remove(RemoveConn);
		}

	
	return true;
}
