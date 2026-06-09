// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_ItemWidget;
class UInv_InventoryItem;

USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()
	
	FInv_SlotAvailability() = default;
	
	FInv_SlotAvailability(int32 ItemIndex, int32 NumItems, bool bHasItem) 
	: Index(ItemIndex)
	, Amount(NumItems)
	, bItemAtIndex(bHasItem) 
	{}
	
	int32 Index { INDEX_NONE };
	
	int32 Amount { 0 };
	
	bool bItemAtIndex { false };
};

USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()
	
	FInv_SlotAvailabilityResult() = default;
	
	TWeakObjectPtr<UInv_InventoryItem> Item;
	
	int32 AvailableSlots { 0 };
	
	int32 Remainder { 0 };
	
	bool bIsStackable { false };
	
	TArray<FInv_SlotAvailability> SlotAvailabilities;
};

USTRUCT()
struct FInv_GridItem
{
	GENERATED_BODY()
	
	TWeakObjectPtr<UInv_InventoryItem> Item;
	
	TWeakObjectPtr<UInv_ItemWidget> ItemWidget;
	
	int32 GridIndex;
};