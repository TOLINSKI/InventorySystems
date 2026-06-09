// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "IContentBrowserSingleton.h"
#include "Inventory.h"
#include "IPropertyTable.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "InventoryManagement/Inv_InventoryComponent.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_GridFragment.h"
#include "Items/Fragments/Inv_IconFragment.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Inv_WidgetUtils.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Item/Inv_ItemWidget.h"

void UInv_InventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	ConstructGrid();
}

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));
	
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

void UInv_InventoryGrid::ConstructGrid()
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

bool UInv_InventoryGrid::IsMatchingCategory(UInv_InventoryItem* Item)
{
	return Item->GetItemSpec().GetItemCategory() == ItemCategory;
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if (!IsMatchingCategory(Item)) return;
	
	FInv_SlotAvailabilityResult Result = GetSlotAvailability(Item->GetItemSpec());
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		const FInv_ItemSpec& ItemSpec = Item->GetItemSpec();
		const FInv_GridFragment* GridFragment = ItemSpec.GetFragment<FInv_GridFragment>();
		const FInv_IconFragment* IconFragment = ItemSpec.GetFragment<FInv_IconFragment>();
		check(GridFragment);
		check(IconFragment);

		UInv_ItemWidget* ItemWidget = CreateWidget<UInv_ItemWidget>(GetOwningPlayer(), ItemWidgetClass);
		FSlateBrush Brush;
		Brush.SetResourceObject(IconFragment->GetTexture());
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.ImageSize = SlotSize * GridFragment->GetGridSpan();
		ItemWidget->GetIconImage()->SetBrush(Brush);
		
		UGridSlot* GridSlot = GridPanel_Items->AddChildToGrid(ItemWidget, 0, 0);
		GridSlot->SetColumnSpan(GridFragment->GetGridSpan().X);
		GridSlot->SetRowSpan(GridFragment->GetGridSpan().Y);
		
		FInv_GridItem GridItem;
		GridItem.Item = Item;
		GridItem.ItemWidget = ItemWidget;
		GridItem.GridIndex = Availability.Index;
		
		Items.Add(MoveTemp(GridItem));
	}
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::GetSlotAvailability(const FInv_ItemSpec& ItemSpec)
{
	// TODO: This is a temp func to add a single item to the cell {0,0}
	
	FInv_SlotAvailabilityResult Result;
	Result.AvailableSlots = 1;

	FInv_SlotAvailability SlotAvailability;
	SlotAvailability.Amount = 1;
	SlotAvailability.Index = 0;
	
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));
	
	return Result;
}



