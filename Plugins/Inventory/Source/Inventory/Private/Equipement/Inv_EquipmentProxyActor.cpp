// Copyright Chaos Box Games 2026, All rights reserved.


#include "Equipement/Inv_EquipmentProxyActor.h"

#include "Equipement/Inv_EquipmentComponent.h"


AInv_EquipmentProxyActor::AInv_EquipmentProxyActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	CharacterMesh->SetupAttachment(SceneRoot);
	
	EquipmentComponent = CreateDefaultSubobject<UInv_EquipmentComponent>("EquipmentComponent");
}

void AInv_EquipmentProxyActor::BeginPlay()
{
	Super::BeginPlay();
}

