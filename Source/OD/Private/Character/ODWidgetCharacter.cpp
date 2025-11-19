// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ODWidgetCharacter.h"


AODWidgetCharacter::AODWidgetCharacter()
{
	
	BodySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodySkeletalMesh->SetupAttachment(GetMesh());
	
	HairSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairSkeletalMesh->SetupAttachment(GetMesh());

	FaceSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceMesh"));
	FaceSkeletalMesh->SetupAttachment(GetMesh());

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponSkeletalMesh->SetupAttachment(GetMesh());

	
	
}

void AODWidgetCharacter::BeginPlay()
{
	Super::BeginPlay();

	BodySkeletalMesh->SetLeaderPoseComponent(GetMesh());
	HairSkeletalMesh->SetLeaderPoseComponent(GetMesh());
	FaceSkeletalMesh->SetLeaderPoseComponent(GetMesh());
	WeaponSkeletalMesh->SetLeaderPoseComponent(GetMesh());
}
