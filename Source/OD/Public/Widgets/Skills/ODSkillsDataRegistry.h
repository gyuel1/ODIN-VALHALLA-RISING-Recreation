// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ODSkillsDataRegistry.generated.h"

class UPaperSprite;
class UODSkillComponent;
class UODListSkillDataObject_Collection;
class UODListSkillDataObject_Base;
/**
 * 
 */
UCLASS()
class OD_API UODSkillsDataRegistry : public UObject
{
	GENERATED_BODY()
	
	
public:
	void InitSkillsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UODListSkillDataObject_Collection*>& GetRegisteredSkillsTabCollections() const {return RegisteredSkillsTabCollections;}
	TArray<UODListSkillDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;
private:
	void InitActiveSkillTab();
	void InitPassiveSkillTab();
	

	UPROPERTY(Transient)
	TArray<UODListSkillDataObject_Collection*> RegisteredSkillsTabCollections;

	UPROPERTY(Transient)
	TWeakObjectPtr<UODSkillComponent> PlayerSkillComponent;
};
