// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_itemSpec.h"
#include "Inv_EquipmentComponent.generated.h"

class UInv_InventoryItem;
class APlayerController;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class INVENTORY_API UInv_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	friend class AInv_EquipmentProxyActor;
	
	UInv_EquipmentComponent();

	virtual void BeginPlay() override;

	void Setup(APlayerController* PlayerController);

	APlayerController* GetOwningPlayer() const;
	
	USkeletalMeshComponent* GetOwningCharacterMesh() const;
	
	void AttachEquipmentActor(UInv_InventoryItem* Item, USkeletalMeshComponent* CharacterMesh);
	void DetachEquipmentActor(UInv_InventoryItem* Item);
	
private:
	UFUNCTION()
	void OnItemEquipped(UInv_InventoryItem* Item);

	UFUNCTION()
	void OnItemUnequipped(UInv_InventoryItem* Item);

	TMap<FGameplayTag, TWeakObjectPtr<AActor>> EquippedActors;
	
	TWeakObjectPtr<APlayerController> OwningPlayer;

	TWeakObjectPtr<AInv_EquipmentProxyActor> ProxyActor;
	
	void TryAttachEquipmentToProxy(UInv_InventoryItem* Item) const;
	void TryDetachEquipmentFromProxy(UInv_InventoryItem* Item) const;
};
