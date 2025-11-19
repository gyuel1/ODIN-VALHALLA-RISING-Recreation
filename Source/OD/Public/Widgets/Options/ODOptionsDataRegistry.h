// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ODOptionsDataRegistry.generated.h"

class UODListDataObject_Base;
class UODListDataObject_Collection;
/**
 * 
 */
UCLASS()
class OD_API UODOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);
	const TArray<UODListDataObject_Collection*>& GetRegisteredOptionsTabCollections() {return RegisteredOptionsTabCollections;}
	TArray<UODListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;
	
private:
	void FindChildListDataRecursively(UODListDataObject_Base* InParentData, TArray<UODListDataObject_Base*>& OutFoundChildListData) const;
	
	void InitGamePlayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab();

	UPROPERTY(Transient)
	TArray<UODListDataObject_Collection*> RegisteredOptionsTabCollections;

	
};
