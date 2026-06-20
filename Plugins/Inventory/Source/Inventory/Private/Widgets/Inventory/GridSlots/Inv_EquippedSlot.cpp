// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Item/Inv_ItemWidget.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnGridSlotBeginHover.Broadcast(this);
}

void UInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	OnGridSlotEndHover.Broadcast(this);
}
	
FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnEquipSlotClicked.Broadcast(this, EquipmentTag);
	}
	
	// TODO Right mouse click?
	// if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	// {
	// 	OnEquipSlotClicked.Broadcast(this, EquipmentTag);
	// }
	
	return FReply::Handled();
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnEquipSlotUnClicked.Broadcast(this, EquipmentTag);
	}
	
	return FReply::Handled();
}

void UInv_EquippedGridSlot::EquipGridItem(FInv_GridItem& InGridItem)
{
	// Handle UI
	SetGridSlotAvailability(EInv_GridSlotState::Occupied);
	SetItemWidget(InGridItem.GetItemWidget());

	// Handle Grid Item
	SetGridItem(&InGridItem);
	InGridItem.SetEquipSlot(this);
}

FInv_GridItem* UInv_EquippedGridSlot::UnequipItem()
{
	// Handle UI
	SetGridSlotAvailability(EInv_GridSlotState::Unoccupied);
	SetItemWidget(nullptr);
	
	// Return Grid Item
	FInv_GridItem* Temp = GetGridItem();
	SetGridItem(nullptr);
	return Temp; 
}
