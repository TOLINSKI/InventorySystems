// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_GridFragment.h"
#include "Items/Fragments/Inv_IconFragment.h"
#include "Widgets/Inv_WidgetUtils.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Item/Inv_ItemWidget.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// if (GetGridItem())
	// {
	// 	SetCursor(EMouseCursor::GrabHand);
	// 	SetGridSlotAvailability(EInv_GridSlotState::Selected);
	// }
	//
	// UInv_InventoryItem* InventoryItem = UInv_InventoryStatics::GetGrabbedItem(GetOwningPlayer());
	// if (!IsValid(InventoryItem)) return;
	//
	// if (InventoryItem->GetItemTag().MatchesTag(EquipmentTag))
	// {
	// 	SetGridSlotAvailability(EInv_GridSlotState::Selected);
	// }
}

void UInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	// if (EquippedGridItem)
	// {
	// 	SetGridSlotAvailability(EInv_GridSlotState::Occupied);
	// }
	//
	// if (IsOccupied()) return;
	//
	// UInv_InventoryItem* InventoryItem = UInv_InventoryStatics::GetGrabbedItem(GetOwningPlayer());
	// if (!IsValid(InventoryItem)) return;
	//
	// if (InventoryItem->GetItemTag().MatchesTag(EquipmentTag))
	// {
	// 	SetGridSlotAvailability(EInv_GridSlotState::Unoccupied);
	// }
}
	
FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnEquipSlotClicked.Broadcast(this, EquipmentTag);
	}
	
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

void UInv_EquippedGridSlot::EquipItem(FInv_GridItem& EquippableGridItem)
{
	// SetGridSlotAvailability(EInv_GridSlotState::Occupied);
	//
	// EquippedGridItem = &EquippableGridItem;
	// EquippableGridItem.SetEquipSlot(this);
	//
	// UUserWidget* ItemWidget = EquippableGridItem.GetItemWidget();
	// ItemWidget->SetVisibility(ESlateVisibility::Visible);
	// SetItemWidget(ItemWidget);
}

FInv_GridItem* UInv_EquippedGridSlot::UnequipItem()
{
	// SetGridSlotAvailability(EInv_GridSlotState::Unoccupied);
	// SetItemWidget(nullptr);
	//
	// FInv_GridItem* Temp = EquippedGridItem;
	// EquippedGridItem = nullptr;
	// return Temp; 
	return GetGridItem();
}
