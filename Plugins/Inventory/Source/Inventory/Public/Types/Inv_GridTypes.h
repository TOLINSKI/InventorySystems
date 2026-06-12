// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h" 
#include "VisualizeTexture.h"

#include "Inv_GridTypes.generated.h"

class UInv_ItemWidget;
class UInv_InventoryItem;

USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()
	
	FInv_SlotAvailability() = default;
	
	FInv_SlotAvailability(int32 ItemIndex, int32 StackCount, bool bItemExists) 
	: ItemIndex(ItemIndex)
	, StackCount(StackCount)
	, bItemExists(bItemExists)
	{}
	
	int32 ItemIndex { INDEX_NONE };
	
	int32 StackCount { 1 };
	
	bool bItemExists { false };
};

USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()
	
	FInv_SlotAvailabilityResult() = default;
	
	UInv_InventoryItem* GetItem() const { return Item.Get(); }
	
	TWeakObjectPtr<UInv_InventoryItem> Item;
	
	bool bIsStackable { false };
	
	int32 Remainder { 0 };
	
	int32 StackCountToAdd { 0 };
	
	TArray<FInv_SlotAvailability> SlotAvailabilities;
	
	bool HasAvailability() const { return SlotAvailabilities.Num() > 0; }
};

USTRUCT()
struct FInv_GridItem
{
	GENERATED_BODY()
	
	FInv_GridItem() = default;
	
	FInv_GridItem(UInv_InventoryItem* InItem, UInv_ItemWidget* InItemWidget, int32 InArrayIndex, const FIntPoint& InGridSpan, int32 InStackCount);
	
	void SetStackCount(const int32 Count) { StackCount = Count; }
	
	int32 AddStackCount(const int32 Count) { StackCount += Count; return StackCount; }
	
	int32 GetStackCount() const { return StackCount; }
	
	bool IsIndexOccupied(const int32 Index) const;
	
	UInv_InventoryItem* GetItem() const;
	
	UInv_ItemWidget* GetItemWidget() const;
	
	FIntPoint GetGridSpan() const { return GridSpan; }
	
	int32 GetArrayIndex() const { return ArrayIndex; }
	
private:
	TWeakObjectPtr<UInv_InventoryItem> Item;
	
	UPROPERTY()
	TObjectPtr<UInv_ItemWidget> ItemWidget;
	
	int32 ArrayIndex { INDEX_NONE };
	
	FIntPoint GridSpan;
	
	int32 StackCount { 1 };
};

USTRUCT()
struct FInv_GridItemGrabber
{
	GENERATED_BODY()
	
	FInv_GridItemGrabber() = default;
	
	bool IsGrabbing() const;
	
	void UpdateGrabbedItemPosition(const FVector2D& MousePosition) const;
	
	void StartGrabbing(const FInv_GridItem& InGridItem, const FVector2D& MouseCursorPosition);

	FInv_GridItem StopGrabbing();
	
	UUserWidget* GetWidget() const;
	
	FInv_GridItem GridItem {};
	
	FVector2D InitGrabPosition {};
	
	FVector2D InitWidgetPosition {};
};