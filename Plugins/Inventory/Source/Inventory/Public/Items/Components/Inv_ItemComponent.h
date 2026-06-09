// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_itemSpec.h"
#include "Inv_ItemComponent.generated.h"

class UInv_InventoryItem;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ItemComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FText GetPickupMessage() const { return PickupMessage; }
	
	FInv_ItemSpec GetItemSpec() const { return ItemSpec; }
	
protected:
	UPROPERTY(Replicated, EditAnywhere, Category="Inventory")
	FInv_ItemSpec ItemSpec;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FText PickupMessage;
};
