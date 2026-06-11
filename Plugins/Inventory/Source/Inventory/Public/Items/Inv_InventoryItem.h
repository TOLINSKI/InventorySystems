// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "Items/Manifest/Inv_ItemSpec.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Inv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool IsSupportedForNetworking() const override { return true; }
	
	void SetItemSpec(const FInv_ItemSpec& Spec);
	
	const FInv_ItemSpec& GetItemSpec() const { return ItemSpec.Get<FInv_ItemSpec>(); }
	
	FInv_ItemSpec& GetItemSpecMutable() { return ItemSpec.GetMutable<FInv_ItemSpec>(); }
	
	void SetStackCount(int32 Count) { TotalStackCount = Count; }
	
	void AddStackCount(int32 Count) { TotalStackCount += Count; }
	
	FGameplayTag GetItemTag() const;
	
private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Inventory", meta = (BaseStruct = "/Script/Inventory.Inv_ItemSpec"))
	FInstancedStruct ItemSpec;
	
	UPROPERTY(Replicated)
	int32 TotalStackCount { 0 };
};
