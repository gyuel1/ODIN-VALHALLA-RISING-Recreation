// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelInstance/ODLevelInstanceBase.h"


AODLevelInstanceBase::AODLevelInstanceBase()
{
	RepGraphCount = -1;
}

AODLevelInstanceBase::AODLevelInstanceBase(const int32 InRepGraphCount)
{
	RepGraphCount = InRepGraphCount;
}

int32 AODLevelInstanceBase::GetRepGraphCount()
{
	return RepGraphCount;
}

void AODLevelInstanceBase::SetRepGraphCount(int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}

