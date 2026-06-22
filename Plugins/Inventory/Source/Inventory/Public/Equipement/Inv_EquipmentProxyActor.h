// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_EquipmentProxyActor.generated.h"

class UInv_EquipmentComponent;
class USkeletalMeshComponent;
class USceneComponent;

UCLASS()
class INVENTORY_API AInv_EquipmentProxyActor : public AActor
{
	GENERATED_BODY()

public:
	AInv_EquipmentProxyActor();

	USkeletalMeshComponent* GetMesh() const { return CharacterMesh; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TObjectPtr<UInv_EquipmentComponent> EquipmentComponent;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;
};
