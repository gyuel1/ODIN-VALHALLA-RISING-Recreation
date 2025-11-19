// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Notify/AN_DestroyActor.h"

#include "AbilitySystemComponent.h"

void UAN_DestroyActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner) return;
	if (!Owner->HasAuthority()) return;

	if (UAbilitySystemComponent* ASC = Owner->FindComponentByClass<UAbilitySystemComponent>())
	{
		ASC->CancelAllAbilities();
	}
	
	Owner->GetWorldTimerManager().SetTimerForNextTick([W=MakeWeakObjectPtr(Owner)]
	{
		if (W.IsValid())
		{
			W->Destroy(true);
		}
	});
}
