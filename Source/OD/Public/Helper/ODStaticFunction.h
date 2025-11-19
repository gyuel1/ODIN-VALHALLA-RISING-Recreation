// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PaperSprite.h"
#include "Character/ODPlayer.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Interface/ODRepGraphInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "LevelInstance/LevelInstanceInterface.h"
#include "LevelInstance/LevelInstanceSubsystem.h"
#include "ReplicationGraph/ODReplicationGraph.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageText.h"


class ALevelInstance;

namespace ODStaticFunc
{
	template <typename T>
	static FString EnumToString(const T& InEnum)
	{
		return StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(InEnum));
	}

	//액터 채널 가져오기
	static int32 GetLevelInstanceRepGraphCount(AActor* InActor)
	{
		UWorld* World = InActor->GetWorld();
		if (!World) return -1;
		
		if (ULevelInstanceSubsystem* LIS = World->GetSubsystem<ULevelInstanceSubsystem>())
		{
			ILevelInstanceInterface* LII =  LIS->GetOwningLevelInstance(InActor->GetLevel());
			if (!LII)
				return -1;
			
			if (UObject* ImplObj = LII ? LII->_getUObject() : nullptr)
			{
				if (ALevelInstance* MakeLevelInstance = Cast<ALevelInstance>(ImplObj))
				{
					if (IODRepGraphInterface* RGI =  Cast<IODRepGraphInterface>(MakeLevelInstance))
					{
						RGI->GetRepGraphCount();
					}
				}
			}
			
		}
		return -1;
	}

	//플레이어 채널 이동
	static void ChangeChannelPlayer(AODPlayer* InPlayer, const int32 InNewChannel,const AActor* SpawnPoint = nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeChannelPlayer"));
		if (!InPlayer || InNewChannel < 0)
			return;

		if (UNetDriver* ND =  InPlayer->GetNetDriver())
		{
			if (UODReplicationGraph* ODRepGraph = Cast<UODReplicationGraph>(ND->GetReplicationDriver()))
			{
				ODRepGraph->ChangeChannelPlayer(InPlayer,InNewChannel,SpawnPoint);
			}
		}
	}

	static int32 OpenChannelPlayer(AODPlayer* InPlayer, const int32 InNewChannel,const AActor* SpawnPoint = nullptr)
	{
		if (!InPlayer || InNewChannel < 0)
			return -1;

		if (UNetDriver* ND =  InPlayer->GetNetDriver())
		{
			if (UODReplicationGraph* ODRepGraph = Cast<UODReplicationGraph>(ND->GetReplicationDriver()))
			{
				return ODRepGraph->OpenChannelPlayer(InPlayer,InNewChannel,SpawnPoint);
			}
		}
		return -1;
	}

	static void CloseChannelPlayer(AODPlayer* InPlayer, const int32 InOldChannel)
	{
		if (!InPlayer || InOldChannel < 0)
			return;

		if (UNetDriver* ND =  InPlayer->GetNetDriver())
		{
			if (UODReplicationGraph* ODRepGraph = Cast<UODReplicationGraph>(ND->GetReplicationDriver()))
			{
			//	ODRepGraph->CloseChannelPlayer(InPlayer,InOldChannel);
			}
		}
	}
	

	

	static void ChangeChannelActor(AActor* InActor , const int32 InNewChannel)
	{
		if (!IsValid(InActor) || !InActor->GetWorld())
			return;

		if (UNetDriver* ND =  InActor->GetWorld()->GetNetDriver())
		{
			if (UODReplicationGraph* ODRepGraph = Cast<UODReplicationGraph>(ND->GetReplicationDriver()))
			{
				//작성중
				//ODRepGraph->ChangeChannelPlayer(InPlayer,InNewChannel,SpawnPoint);
			}
		}
	}
	

	//범용 SphereTraces
	template <typename T>
	static TArray<T*> DetectSphereInRange(AActor* InOwner, const float& InRange,const ECollisionChannel InCollisionChannel = ECC_GameTraceChannel3 ,const FVector& Offset = FVector())
	{
		
		TSet<T*> MakeTargetSet = {};
		if (!InOwner || InRange <= 0.f)
			return MakeTargetSet.Array();

		TArray<FHitResult> OutActors;
		TArray<AActor*> ActorsToIgnore;
		//ActorsToIgnore.Add(InOwner);

		//ECC_GameTraceChannel2 / DetectTarget
		const TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(InCollisionChannel);

		const FVector Center = InOwner->GetActorLocation() + Offset;
		const float Radius = InRange;

		UKismetSystemLibrary::SphereTraceMulti(
			InOwner,
			Center,
			Center,
			Radius,
			TraceChannel,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			OutActors,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			3.0f
		);
		
		for (FHitResult& HitResult : OutActors)
		{
			if (T* HitActor = Cast<T>(HitResult.GetActor()))
			{
				if (InOwner->IsA(AODPlayer::StaticClass()))
				{
					if (HitActor->IsA(AODPlayer::StaticClass()))
						continue;
				}
				MakeTargetSet.Add(HitActor);
			}
		}
		return MakeTargetSet.Array();
	}

	//범용 BoxTrace
	template <typename T>
	static TArray<T*> DetectBoxInRange(AActor* InOwner, const FVector InStart, const FVector InEnd,
	                                   const FVector InHalfSize,
	                                   const ECollisionChannel InCollisionChannel = ECC_GameTraceChannel3,
	                                   const FVector& Offset = FVector())
	{
		TSet<T*> MakeTargetSet = {};
		if (!InOwner)
			return MakeTargetSet.Array();

		TArray<FHitResult> OutActors;
		TArray<AActor*> ActorsToIgnore;
		//ActorsToIgnore.Add(InOwner);

		//ECC_GameTraceChannel2 / DetectTarget
		const TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(InCollisionChannel);
		
		//box
		UKismetSystemLibrary::BoxTraceMulti(
			InOwner,
			InStart,
			InEnd,
			InHalfSize,
			InOwner->GetActorRotation(),
			TraceChannel,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			OutActors,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.f );
		

		for (FHitResult& HitResult : OutActors)
		{
			if (T* HitActor = Cast<T>(HitResult.GetActor()))
			{
				if (InOwner->IsA(AODPlayer::StaticClass()))
				{
					if (HitActor->IsA(AODPlayer::StaticClass()))
						continue;
				}
				MakeTargetSet.Add(HitActor);
			}
		}
		return MakeTargetSet.Array();
	}
}

namespace ODUIStaticFunc
{
	static FSlateBrush LoadSynchronousSprintAndConvertSlateImage(UPaperSprite* InSprite)
	{
		FSlateBrush Brush;
		
		if (!InSprite)
			return Brush;
		
		UTexture2D* AtlasTexture = InSprite->GetBakedTexture();
		
		if (!AtlasTexture)
		{
			UE_LOG(LogTemp, Error, TEXT("Sprite '%s' has no BakedTexture!"), *InSprite->GetName());
			return Brush;
		}

		// 기본값
		FVector2D UV = FVector2D::ZeroVector;
		FVector2D Size = FVector2D::ZeroVector;

		// Protected 멤버 Reflection 접근
		if (FProperty* UVProp = UPaperSprite::StaticClass()->FindPropertyByName(TEXT("BakedSourceUV")))
		{
			void* SpritePtr = (void*)InSprite;
			UVProp->CopyCompleteValue(&UV, UVProp->ContainerPtrToValuePtr<void>(SpritePtr));
		}

		if (FProperty* SizeProp = UPaperSprite::StaticClass()->FindPropertyByName(TEXT("BakedSourceDimension")))
		{
			void* SpritePtr = (void*)InSprite;
			SizeProp->CopyCompleteValue(&Size, SizeProp->ContainerPtrToValuePtr<void>(SpritePtr));
		}

		// Atlas 크기
		FVector2D AtlasSize(AtlasTexture->GetSizeX(), AtlasTexture->GetSizeY());

		// UV 계산 (0~1 기준)
		FVector2D UVMin = UV / AtlasSize;
		FVector2D UVMax = (UV + Size) / AtlasSize;

		// 브러시 생성
		Brush.SetResourceObject(AtlasTexture);
		Brush.ImageSize = Size;
		Brush.SetUVRegion(FBox2D(UVMin, UVMax));

		return Brush;
	}

	static void InsertAtTop(UVerticalBox* VB, UODWidget_DamageText* NewWidget, int32 MaxCount = -1, bool bReverseOrder = false)
	{
		if (!VB || !NewWidget) return;

		// 순서 반전 처리
		if (bReverseOrder)
		{
			VB->SetRenderScale(FVector2D(1.f, -1.f));         // 전체 박스 뒤집기
			NewWidget->SetRenderScale(FVector2D(1.f, -1.f));  // 텍스트 다시 정방향으로 복원
		}
		else
		{
			VB->SetRenderScale(FVector2D(1.f, 1.f));
			NewWidget->SetRenderScale(FVector2D(1.f, 1.f));
		}

		// 새 DamageText 추가
		UVerticalBoxSlot* NewSlot = VB->AddChildToVerticalBox(NewWidget);
		if (NewSlot)
		{
			NewSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);
			NewSlot->SetPadding(FMargin(0, 2.f, 0, 0)); // 기본 간격
		}

		// 최대 개수 제한
		if (MaxCount > 0 && VB->GetAllChildren().Num() > MaxCount)
		{
			VB->RemoveChildAt(0);
		}

		const int32 NumChildren = VB->GetAllChildren().Num();
		
		const int32 NewIndex = NumChildren - 1;

		for (int32 i = 0; i < NumChildren; ++i)
		{
			if (UODWidget_DamageText* ChildText = Cast<UODWidget_DamageText>(VB->GetChildAt(i)))
			{
				if (i == NewIndex) continue; // 새로 들어온 것만 제외

				FSlateFontInfo FontInfo = ChildText->DamageText->GetFont();
				FontInfo.Size = 15;
				ChildText->DamageText->SetFont(FontInfo);
				ChildText->SetRenderOpacity(0.5f);

				if (ChildText->NextIndexDamageAnim)
				{
					ChildText->PlayAnimation(ChildText->NextIndexDamageAnim);
				}
			}
		}

		if (UODWidget_DamageText* NewDamage = Cast<UODWidget_DamageText>(NewWidget))
		{
			if (NewDamage->FirstIndexDamageAnim)
			{
				NewDamage->PlayAnimation(NewDamage->FirstIndexDamageAnim);
			}
		}
	}
}


