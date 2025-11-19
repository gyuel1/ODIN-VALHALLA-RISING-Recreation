// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryDataAsset/GenericGA/OD_PDA_GenericGA.h"


UOD_PDA_GenericGA::UOD_PDA_GenericGA()
{
	ODCharacterType = EODCharacterType::None;
}

FPrimaryAssetId UOD_PDA_GenericGA::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("GenericGA",GetFName());
}
