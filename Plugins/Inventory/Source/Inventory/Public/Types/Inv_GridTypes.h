// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h" 
#include "GameplayTagContainer.h"

#include "Inv_GridTypes.generated.h"

class UInv_EquippedGridSlot;
struct FGameplayTag;
class UInv_ItemPopUp;
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
	
	FInv_SlotAvailability(int32 ItemIndex, bool bItemExists) 
	: ItemIndex(ItemIndex)
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
	
	FInv_GridItem(UInv_InventoryItem* InItem, UInv_ItemWidget* InItemWidget, int32 InArrayIndex, const FIntPoint& InGridSpan, int32 InStackCount, int32 InMaxStackCount);
	
	bool operator ==(const FInv_GridItem& Other) const;
	
	void SetStackCount(const int32 Count) { StackCount = Count; UpdateStackCountUI(); }
	
	int32 AddStackCount(const int32 Count) { StackCount += Count; UpdateStackCountUI(); return StackCount; }

	int32 SubtractStackCount(const int32 Count) { StackCount -= Count; UpdateStackCountUI(); return StackCount; }
	
	int32 GetStackCount() const { return StackCount; }

	int32 GetStackableAmount() const { return FMath::Max(MaxStackCount - StackCount, 0); }
	
	int32 GetMaxStackCount() const { return MaxStackCount; }
	
	bool HasRoomToStack() const { return MaxStackCount > StackCount; }
	
	bool IsIndexOccupied(const int32 Index) const;
	
	UInv_InventoryItem* GetItem() const;
	
	UInv_ItemWidget* GetItemWidget() const;
	
	FIntPoint GetGridSpan() const { return GridSpan; }
	
	int32 GetIndex() const { return ArrayIndex; }
	
	void SetArrayIndex(int32 Index) { ArrayIndex = Index; }
	
	bool IsStackable() const;
	
	bool IsConsumable() const;
	
	FGameplayTag GetItemTag() const;
	
	void SetEquipSlot(UInv_EquippedGridSlot* EquippedGridSlot) { LastEquipSlot = EquippedGridSlot; }
	UInv_EquippedGridSlot* GetEquippedGridSlot() const { return LastEquipSlot; }
	
	bool IsValid() const;
	
private:
	TWeakObjectPtr<UInv_InventoryItem> Item;
	
	UPROPERTY()
	TObjectPtr<UInv_ItemWidget> ItemWidget;
	
	int32 ArrayIndex { INDEX_NONE };
	
	FIntPoint GridSpan;
	
	int32 StackCount { 1 };
	
	int32 MaxStackCount { 1 };
	
	void UpdateStackCountUI() const;

	UPROPERTY()
	TObjectPtr<UInv_EquippedGridSlot> LastEquipSlot;
};

USTRUCT()
struct FInv_GridGrabQuery
{
	GENERATED_BODY()
	
	FInv_GridGrabQuery() = default;
	
	void StartGrabbing(FInv_GridItem& InGridItem, const FVector2D& WidgetPosition, const FVector2D& MouseCursorPosition);
	
	bool IsGrabbing() const { return bIsGrabbing && GetGridItem().IsValid(); }
	
	bool HasFoundPossibleGridIndex() const { return LastPossibleIndex != INDEX_NONE; }
	
	void UpdateGrabbedItemPosition(const FVector2D& MousePosition) const;
	
	void ResetQuery();

	TArray<FInv_GridItem*>& GetBlockingGridItems() { return BlockingGridItems;}

	void StopGrabbing();
	
	UUserWidget* GetWidget() const;
	
	const FInv_GridItem& GetGridItem() const { return GrabbedItem; } 
	
	FInv_GridItem& GetMutableGridItem() { return GrabbedItem; }
	
	FInv_GridItem* GetStackableGridItem() const { return StackableGridItem; }
	
	int32 GetPossibleIndex() const { return LastPossibleIndex; }
	
	void SetPossibleIndex(int32 Index) { LastPossibleIndex = Index; }
	
	void SetStackableGridItem(FInv_GridItem* GridItem) { StackableGridItem = GridItem; }
	
	void ResetIndex();
	
	FVector2D GetRelativeWidgetPosition(const FVector2D& MousePosition) const;
	
	bool CanReturnGrabbedItemToGrid() const { return bCanReturnGrabbedItemToGrid; }
	void SetCanReturnGrabbedItemToGrid(bool bCanReturn) { bCanReturnGrabbedItemToGrid = bCanReturn;}
	
private:
	bool bIsGrabbing { false };
	
	FVector2D InitMousePosition {};
	
	FVector2D InitWidgetPosition {};
	
	FVector2D DistMouseToWidget {};
	
	int32 LastPossibleIndex { INDEX_NONE };
	
	UPROPERTY()
	FInv_GridItem GrabbedItem;
	
	FInv_GridItem* StackableGridItem {};
	
	int32 StackableAmount { 0 };
	
	TArray<FInv_GridItem*> BlockingGridItems {};
	
	bool bCanReturnGrabbedItemToGrid { true };
};

UENUM()
enum class EInv_GridSlotQuadrant
{
	None,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
};

USTRUCT()
struct FInv_GridPopUp
{
	GENERATED_BODY()

	FInv_GridPopUp() = default;
	
	void Init(FInv_GridItem& GridItem, UUserWidget* PopUpMenu = nullptr);
	
	UUserWidget* GetWidget() const { return PopUpWidget; }
	
	int32 GetIndex() const { return TargetGridItem->GetIndex(); }
	
	FInv_GridItem* GetGridItem() const { return TargetGridItem; }
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> PopUpWidget { nullptr };
	
	FInv_GridItem* TargetGridItem { nullptr };
};

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	Disabled
};