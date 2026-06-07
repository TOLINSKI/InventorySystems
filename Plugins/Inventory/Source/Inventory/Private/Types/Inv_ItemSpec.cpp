// Copyright Chaos Box Games 2026, All rights reserved.

#include "Types/Inv_itemSpec.h"

#include "Items/Inv_InventoryItem.h"

UInv_InventoryItem* FInv_ItemSpec::CreateItem(UObject* Outer) const
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(Outer);
	Item->SetItemSpec(*this);
	return Item;
}
