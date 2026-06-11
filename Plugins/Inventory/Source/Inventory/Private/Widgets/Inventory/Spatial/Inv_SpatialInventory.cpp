// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Inventory.h"
#include "Components/WidgetSwitcher.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetGridCategory(EInv_ItemCategory::Equippable);
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::GetGridAvailability(UInv_ItemComponent* ItemComponent) const
{
	switch (ItemComponent->GetItemSpec().GetItemCategory())
	{
	case EInv_ItemCategory::Equippable:
		return Grid_Equippables->GetSlotAvailability(ItemComponent);
	case EInv_ItemCategory::Consumable:
		return Grid_Consumables->GetSlotAvailability(ItemComponent);
	case EInv_ItemCategory::Craftable:
		return Grid_Craftables->GetSlotAvailability(ItemComponent);
	default:
		UE_LOG(LogInventory, Error, TEXT("ItemComponent doesn't have a valid Item Category."))
		return FInv_SlotAvailabilityResult();
	}
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

