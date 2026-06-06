// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_InventoryGridWidget.h"

#include "Components/GridPanel.h"
#include "Widgets/Inv_WidgetUtils.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"

void UInv_InventoryGridWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	PopulateGrid();
}

void UInv_InventoryGridWidget::PopulateGrid()
{
	if (!ensure(IsValid(GridSlotClass))) return;
	
	if (IsDesignTime())
	{
		GridSlots.Empty();
		GridPanel_Items->ClearChildren();
	}
	
	GridSlots.Reserve(Rows * Columns);
	
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Column = 0; Column < Columns; ++Column)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			GridSlot->SlotSize = SlotSize;
			GridPanel_Items->AddChildToGrid(GridSlot, Row, Column);
			
			GridSlot->ArrayIndex = UInv_WidgetUtils::GridPositionToArrayIndex({Row, Column}, Columns);
			GridSlots.Add(GridSlot);
		}
	}
}
