// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Inventory.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "InventoryManagement/Inv_InventoryComponent.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_GridFragment.h"
#include "Items/Fragments/Inv_IconFragment.h"
#include "Items/Fragments/Inv_StackFragment.h"
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
	InventoryComponent->OnStackChanged.AddDynamic(this, &ThisClass::AddResult);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::GetSlotAvailability(const UInv_ItemComponent* ItemComponent) const
{
	return GetSlotAvailability(ItemComponent->GetItemSpec());
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
			GridSlot->SetSlotSize(GridSlotSize);
			GridPanel_Items->AddChildToGrid(GridSlot, Row, Column);
			GridSlot->SetArrayIndex(UInv_WidgetUtils::GridPositionToIndex({Column, Row}, Columns));
			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGrid::IsMatchingCategory(UInv_InventoryItem* Item)
{
	return Item->GetItemSpec().GetItemCategory() == ItemCategory;
}

UInv_ItemWidget* UInv_InventoryGrid::CreateItemWidget(const FInv_GridFragment* GridFragment, const FInv_IconFragment* IconFragment)
{
	UInv_ItemWidget* ItemWidget = CreateWidget<UInv_ItemWidget>(GetOwningPlayer(), ItemWidgetClass);
	FSlateBrush Brush;
	Brush.SetResourceObject(IconFragment->GetTexture());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = SlotSize * GridFragment->GetGridSpan();
	ItemWidget->GetIconImage()->SetBrush(Brush);
	return ItemWidget;
}

void UInv_InventoryGrid::AddGridItem(const FInv_GridItem& GridItem)
{
	const int32 Index = GridItem.GetIndex();
	const FIntPoint GridPosition = UInv_WidgetUtils::IndexToGridPosition(Index, Columns);
	const FIntPoint GridSpan = GridItem.GetGridSpan();
	UGridSlot* ItemSlot = GridPanel_Items->AddChildToGrid(GridItem.GetItemWidget(), GridPosition.Y, GridPosition.X);
	ItemSlot->SetColumnSpan(GridSpan.X);
	ItemSlot->SetRowSpan(GridSpan.Y);

	UInv_WidgetUtils::ForEach2D(GridSlots, Index, GridSpan, Columns, [](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetOccupied(true);
	});
	
	GridItems.Add(GridItem);
}

void UInv_InventoryGrid::Stack(const FInv_SlotAvailability& SlotAvailability)
{
	FInv_GridItem* GridItem = GetGridItemAtIndexMutable(SlotAvailability.ItemIndex);
	check(GridItem != nullptr);
	
	const int32 StackCount = GridItem->AddStackCount(SlotAvailability.StackCount);
	GridItem->GetItemWidget()->StackCountChanged(StackCount);
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if (!IsMatchingCategory(Item)) return;
	
	FInv_SlotAvailabilityResult Result = GetSlotAvailability(Item->GetItemSpec());
	Result.Item = Item;
	AddResult(Result);
}

void UInv_InventoryGrid::AddResult(const FInv_SlotAvailabilityResult& Result)
{
	UInv_InventoryItem* Item = Result.Item.Get();
	check(IsValid(Item));
	
	if (!IsMatchingCategory(Item)) return;
	
	for (const auto& SlotAvailability : Result.SlotAvailabilities)
	{
		if (SlotAvailability.bItemExists)
		{
			Stack(SlotAvailability);
			continue;
		}
		
		const FInv_ItemSpec& ItemSpec = Item->GetItemSpec();
		const FInv_GridFragment* GridFragment = ItemSpec.GetFragment<FInv_GridFragment>();
		const FInv_IconFragment* IconFragment = ItemSpec.GetFragment<FInv_IconFragment>();
		check(GridFragment);
		check(IconFragment);
		
		UInv_ItemWidget* ItemWidget = CreateItemWidget(GridFragment, IconFragment);
		AddGridItem(FInv_GridItem(Item, ItemWidget, SlotAvailability.ItemIndex, GridFragment->GetGridSpan(), SlotAvailability.StackCount));
		
		if (Result.bIsStackable)
		{
			ItemWidget->StackCountChanged(SlotAvailability.StackCount);
		}
	}
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::GetSlotAvailability(const FInv_ItemSpec& ItemSpec) const
{
	TSet<int32> Visited;
	FInv_SlotAvailabilityResult Result;
	
	const FInv_GridFragment* GridFragment = ItemSpec.GetFragment<FInv_GridFragment>();
	const FInv_StackFragment* StackFragment = ItemSpec.GetFragment<FInv_StackFragment>();
	
	Result.bIsStackable = StackFragment != nullptr;
	Result.Remainder = StackFragment != nullptr ? StackFragment->GetStackCount() : 1;
	
	// Handle all stackable slots first
	if (Result.bIsStackable)
	{
		for (int32 i = 0; i < GridSlots.Num(); ++i)
		{
			if (const FInv_GridItem* GridItem = GetGridItemAtIndex(i))
			{
				if (Visited.Contains(i)) continue;
				Visited.Add(i);
				
				UInv_InventoryItem* ExistingItem = GridItem->GetItem();
				if (ExistingItem->GetItemTag() != ItemSpec.GetItemTag()) continue;
				
				const FInv_ItemSpec& FoundSpec = ExistingItem->GetItemSpec();
				const int32 MaxStackCount = FoundSpec.GetFragment<FInv_StackFragment>()->GetMaxStackCount();
				const int32 OccupiedStackCount = GridItem->GetStackCount();
				const int32 AvailableStackCount = MaxStackCount - OccupiedStackCount;
				check(AvailableStackCount >= 0);
				if (AvailableStackCount == 0) continue;
				
				const int32 AmountToStack = FMath::Min(Result.Remainder, AvailableStackCount);
				Result.StackCountToAdd += AmountToStack;
				Result.Remainder -= AmountToStack;
				Result.SlotAvailabilities.Emplace(
					FInv_SlotAvailability{i, AmountToStack, true });
				if (Result.Remainder == 0)
				{
					return Result;
				}
			}
		}
	}
	
	// All stackable options handled, look for empty spots
	for (int32 i = 0; i < GridSlots.Num(); ++i)
	{
		if (Visited.Contains(i)) continue;
		Visited.Add(i);
		
		const FIntPoint GridSpan = GridFragment->GetGridSpan();
		if (!CanFitRange(i, GridSpan)) continue;
		
		TSet<int32> Occupied = FindOccupiedIndices(i, GridSpan);
		Visited.Append(Occupied);
		if (Occupied.Num() > 0) continue;
		
		TSet<int32> ToOccupy = OccupyIndices(i, GridSpan);
		Visited.Append(ToOccupy);
		
		if (Result.bIsStackable)
		{
			const int32 AmountToStack = FMath::Min(StackFragment->GetMaxStackCount(), Result.Remainder);
			Result.StackCountToAdd += AmountToStack;
			Result.Remainder -= AmountToStack;
			Result.SlotAvailabilities.Emplace(
				FInv_SlotAvailability{i, AmountToStack, false });
			if (Result.Remainder == 0)
			{
				return Result;
			}
			
			continue;
		}
		
		Result.StackCountToAdd = 1;
		Result.Remainder = 0;
		Result.SlotAvailabilities.Emplace(
			FInv_SlotAvailability{i, 1, false });
		break;
	}
	
	return Result;
}

bool UInv_InventoryGrid::IsIndexOccupied(int32 Index) const
{
	return GridItems.ContainsByPredicate([Index](const FInv_GridItem& Item){ return Item.IsIndexOccupied(Index); });
}

bool UInv_InventoryGrid::CanFitRange(int32 Index, const FIntPoint& Range2D) const
{
	const FIntPoint GridPosition = UInv_WidgetUtils::IndexToGridPosition(Index, Columns);
	const FIntPoint RightCorner = GridPosition + (Range2D - FIntPoint(1,1));
	const int32 RightCornerIndex = UInv_WidgetUtils::GridPositionToIndex(RightCorner, Columns);
	return GridSlots.IsValidIndex(RightCornerIndex);
}

TSet<int32> UInv_InventoryGrid::FindOccupiedIndices(int32 Index, const FIntPoint& Range2D) const
{
	TSet<int32> Occupied;
	
	UInv_WidgetUtils::ForEach2D(GridSlots, Index, Range2D, Columns, [&Occupied](const UInv_GridSlot* GridSlot)
	{
		if (GridSlot->IsOccupied())
		{
			Occupied.Add(GridSlot->GetArrayIndex());
		}
	});
	
	return Occupied;
}

TSet<int32> UInv_InventoryGrid::OccupyIndices(int32 Index, const FIntPoint& Range2D) const
{
	TSet<int32> Occupied;
	
	UInv_WidgetUtils::ForEach2D(GridSlots, Index, Range2D, Columns, [&Occupied](const UInv_GridSlot* GridSlot)
	{
		const int32 ArrayIndex = GridSlot->GetArrayIndex();
		Occupied.Add(ArrayIndex);
	});
	
	return Occupied;
}

UInv_InventoryItem* UInv_InventoryGrid::GetItemObjectAtIndex(int32 Index) const
{
	const FInv_GridItem* Item = GridItems.FindByPredicate([Index](const FInv_GridItem& Item)
	{
		return Item.GetIndex() == Index;
	});
	
	return Item != nullptr ? Item->GetItem() : nullptr;
}

const FInv_GridItem* UInv_InventoryGrid::GetGridItemAtIndex(int32 Index) const
{
	return GridItems.FindByPredicate([Index](const FInv_GridItem& Item)
	{
		return Item.GetIndex() == Index;
	});
}

FInv_GridItem* UInv_InventoryGrid::GetGridItemAtIndexMutable(int32 Index)
{
	return GridItems.FindByPredicate([Index](const FInv_GridItem& Item)
{
	return Item.GetIndex() == Index;
});
}

