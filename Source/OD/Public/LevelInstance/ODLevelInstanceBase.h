// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interface/ODRepGraphInterface.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ODLevelInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class OD_API AODLevelInstanceBase : public ALevelInstance, public IODRepGraphInterface
{
	GENERATED_BODY()

public:
	AODLevelInstanceBase();
	AODLevelInstanceBase(const int32 InRepGraphCount);

	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(int32 InRepGraph) override;

protected:
	template<typename T>
	T* FindActorInMyLevel();

	UPROPERTY()
	int32 RepGraphCount;
	
	
};

template <typename T>
T* AODLevelInstanceBase::FindActorInMyLevel()
{
	UWorld* World = GetWorld();
	if (!World)
		return nullptr;
	
	if (ULevelInstanceSubsystem* Sub = World->GetSubsystem<ULevelInstanceSubsystem>())
	{
		if (ULevel* LoadLevel = Sub->GetLevelInstanceLevel(this))
		{
			const TArray<TObjectPtr<AActor>>& Actors = LoadLevel->Actors;
			for (AActor* Actor : Actors)
			{
				if (!IsValid(Actor))
					continue;
				
				if (T* AsT = Cast<T>(Actor))
				{
					return AsT; 
				}
			}
		}
	}
	return nullptr;
}
