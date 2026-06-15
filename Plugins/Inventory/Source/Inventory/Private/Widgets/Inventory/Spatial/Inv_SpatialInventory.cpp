// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Inventory.h"
#include "Components/WidgetSwitcher.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Grid_Equippables->OnUnClicked.BindUObject(this, &UInv_SpatialInventory::OnGridUnclicked);
	Grid_Consumables->OnUnClicked.BindUObject(this, &UInv_SpatialInventory::OnGridUnclicked);
	Grid_Craftables->OnUnClicked.BindUObject(this, &UInv_SpatialInventory::OnGridUnclicked);
	
	SwitchGridCategory(EInv_ItemCategory::Equippable);
}

FReply UInv_SpatialInventory::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		
	}
	
	return FReply::Handled();
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

void UInv_SpatialInventory::OnGridUnclicked()
{
	UInv_InventoryGrid* ActiveGrid = Cast<UInv_InventoryGrid>(Grid_Switcher->GetActiveWidget());
	check(IsValid(ActiveGrid));
	
	if (ShouldDropItem())
	{
		ActiveGrid->DropLastGrabbedItem();
	}
}

