// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Components/WidgetSwitcher.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetGridCategory(EInv_ItemCategory::Equippable);
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::GetSlotAvailability(UInv_ItemComponent* ItemComponent) const
{
	FInv_SlotAvailabilityResult Result;
	Result.AvailableSlots = 1;
	return Result;
}

void UInv_SpatialInventory::SetGridCategory(EInv_ItemCategory Category)
{
	switch (Category) {
	case EInv_ItemCategory::None:
		break;
	case EInv_ItemCategory::Equippable:
		Grid_Switcher->SetActiveWidget(Grid_Equippables);
		break;
	case EInv_ItemCategory::Consumable:
		Grid_Switcher->SetActiveWidget(Grid_Consumables);
		break;
	case EInv_ItemCategory::Craftable:
		Grid_Switcher->SetActiveWidget(Grid_Craftables);
		break;
	}
	
	OnCategorySelected(Category);
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::HasAvailableSpaceForItem(UInv_InventoryItem* Item) const
{
	FInv_SlotAvailabilityResult Result;
	Result.AvailableSlots = 1;
	return Result;
}

