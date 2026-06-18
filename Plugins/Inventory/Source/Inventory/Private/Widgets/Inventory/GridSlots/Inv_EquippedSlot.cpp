// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsOccupied()) return;
	
	UInv_InventoryItem* InventoryItem = UInv_InventoryStatics::GetGrabbedItem(GetOwningPlayer());
	if (!IsValid(InventoryItem)) return;

	if (InventoryItem->GetItemTag().MatchesTag(EquipmentTag))
	{
		SetGridSlotState(EInv_GridSlotState::Occupied);
	}
}

void UInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UInv_InventoryItem* InventoryItem = UInv_InventoryStatics::GetGrabbedItem(GetOwningPlayer());
	if (!IsValid(InventoryItem)) return;

	if (InventoryItem->GetItemTag().MatchesTag(EquipmentTag))
	{
		SetGridSlotState(EInv_GridSlotState::Unoccupied);
	}
}
	
FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	return FReply::Unhandled();
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	return FReply::Unhandled();
}
