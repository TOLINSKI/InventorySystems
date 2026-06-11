// Copyright Chaos Box Games 2026, All rights reserved.

#include "Types/Inv_GridTypes.h"

#include "Items/Inv_InventoryItem.h"
#include "Widgets/Item/Inv_ItemWidget.h"

FInv_GridItem::FInv_GridItem(UInv_InventoryItem* InItem, UInv_ItemWidget* InItemWidget, int32 Index,
	const FIntPoint& InGridSpan, int32 InStackCount)
: Item(InItem)
, ItemWidget(InItemWidget)
, GridIndex(Index)
, GridSpan(InGridSpan) 
, StackCount(InStackCount)
{}

bool FInv_GridItem::IsIndexOccupied(const int32 Index) const
{
	return GridIndex == Index;
}

UInv_InventoryItem* FInv_GridItem::GetItem() const
{
	return Item.Get();
}

UInv_ItemWidget* FInv_GridItem::GetItemWidget() const
{
	return ItemWidget.Get();
}
