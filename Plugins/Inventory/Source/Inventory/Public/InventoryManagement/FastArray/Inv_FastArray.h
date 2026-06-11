// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayTagContainer.h"

#include "Inv_FastArray.generated.h"

class UInv_ItemComponent;
class UInv_InventoryItem;

/** A single inventory item with fast array serialization  */
USTRUCT(BlueprintType)
struct FInv_InventoryFastArrayItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	friend UInv_InventoryItem;
	
	FInv_InventoryFastArrayItem() = default;

	UInv_InventoryItem* Get() const { return ItemObject; }
	
	void SetObject(UInv_InventoryItem* Item) { ItemObject = Item; }
	
private:
	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> ItemObject { nullptr };
};

/** A list of inventory items with fast array serialization */
USTRUCT(BlueprintType)
struct FInv_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()
	
	FInv_InventoryFastArray() = default;
	
	explicit FInv_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	TArray<UInv_InventoryItem*> GetAllItems() const;
	
	// Begin FFastArraySerializer Optional Functions
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	// End FFastArraySerializer Optional Functions
	
	// Net Delta Serialize Implementation
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FastArrayDeltaSerialize<FInv_InventoryFastArrayItem, FInv_InventoryFastArray>( Items, DeltaParms, *this );
	}
	
	UInv_InventoryItem* AddItem(UInv_ItemComponent* ItemComponent);
	UInv_InventoryItem* AddItem(UInv_InventoryItem* Item);
	void RemoveItem(UInv_InventoryItem* Item);
	
	UInv_InventoryItem* FindItemByTag(const FGameplayTag& Tag);
	
private:
	// Replicated list of items
	UPROPERTY()
	TArray<FInv_InventoryFastArrayItem> Items;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent { nullptr };
};

template<>
struct TStructOpsTypeTraits< FInv_InventoryFastArray > : TStructOpsTypeTraitsBase2< FInv_InventoryFastArray >
{
	enum 
	{
		WithNetDeltaSerializer = true,
   };
};

