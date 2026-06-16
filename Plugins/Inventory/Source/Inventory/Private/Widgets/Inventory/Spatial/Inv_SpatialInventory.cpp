// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Inventory.h"
#include "Components/WidgetSwitcher.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Grid_Equippables->OnUnClicked.BindUObject(this, &UInv_SpatialInventory::OnReleasedGridItem);
	Grid_Consumables->OnUnClicked.BindUObject(this, &UInv_SpatialInventory::OnReleasedGridItem);
	Grid_Craftables->OnUnClicked.BindUObject(this, &UInv_SpatialInventory::OnReleasedGridItem);
	
	SwitchGridCategory(EInv_ItemCategory::Equippable);
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::GetGridAvailability(UInv_ItemComponent* ItemComponent) const
{
	const UInv_InventoryGrid* GridToCheck = GetGridByCategory(ItemComponent->GetItemSpec().GetItemCategory());
	check(IsValid(GridToCheck));
	return GridToCheck->GetSlotAvailability(ItemComponent);
}

void UInv_SpatialInventory::SwitchGridCategory(EInv_ItemCategory Category)
{
	auto GridToActivate = GetGridByCategory(Category);
	Grid_Switcher->SetActiveWidget(GridToActivate);
	
	OnCategorySelected(Category);
}

UInv_InventoryGrid* UInv_SpatialInventory::GetGridByCategory(EInv_ItemCategory Category) const
{
	switch (Category) {
	case EInv_ItemCategory::None:
		UE_LOG(LogInventory, Error, TEXT("ItemComponent doesn't have a valid Item Category."))
		return nullptr;
	case EInv_ItemCategory::Equippable: return Grid_Equippables;
	case EInv_ItemCategory::Consumable: return Grid_Consumables;
	case EInv_ItemCategory::Craftable: return Grid_Craftables;
	}
	return nullptr;
}

UInv_InventoryGrid* UInv_SpatialInventory::GetActiveGrid() const
{
	return Cast<UInv_InventoryGrid>(Grid_Switcher->GetActiveWidget());
}

void UInv_SpatialInventory::OnReleasedGridItem()
{
	if (ShouldDropItem())
	{
		GetActiveGrid()->DropLastGrabbedItem();
	}
}

