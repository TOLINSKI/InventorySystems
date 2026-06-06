// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_InventorySpatialWidget.h"

#include "Components/WidgetSwitcher.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGridWidget.h"

void UInv_InventorySpatialWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetGridCategory(EInv_ItemCategory::Equippable);
}

void UInv_InventorySpatialWidget::SetGridCategory(EInv_ItemCategory Category)
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

