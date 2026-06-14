// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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

void UInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!GrabbedQuery.IsGrabbing()) return;
	
	GrabbedQuery.UpdateGrabbedItemPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));
	const EInv_GridSlotQuadrant GridSlotQuadrant = GetGridSlotQuadrant(GrabbedQuery.GetWidget());
	UE_LOG(LogInventory, Display, TEXT("Grid slot quadrant: %s"), *UEnum::GetValueAsString(GridSlotQuadrant));
	
	FInv_GridItem GrabbedGridItem = GrabbedQuery.GetGridItem();
	const FIntPoint GrabbedGridSpan = GrabbedGridItem.GetGridSpan();
	
	if (GrabbedQuery.Result.HasFoundPossibleIndex())
	{
		UnoccupyGridSlotsInRange(GrabbedQuery.Result.LastPossibleIndex, GrabbedGridSpan);
	}
	
	for (const auto& BlockingGridItem : GrabbedQuery.Result.BlockingGridItems)
	{
		UInv_WidgetUtils::ForEach2D(GridSlots, BlockingGridItem->GetArrayIndex(), BlockingGridItem->GetGridSpan(), Columns, [](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetGridSlotState(EInv_GridSlotState::Occupied);
		});
	}
	GrabbedQuery.Result.BlockingGridItems.Empty();
	
	// Positioning
	const FVector2D GrabbedCenter = UInv_WidgetUtils::GetWidgetCenter(GrabbedGridItem.GetItemWidget());
	const FIntPoint GridCoordinates = GetGridCoordinatesByQuadrant(GrabbedCenter, GrabbedGridSpan, GridSlotQuadrant); \
	const int32 Index = UInv_WidgetUtils::GridCoordinatesToIndex(GridCoordinates, Columns);
	if (!CanFitRange(Index, GrabbedGridSpan)) return;
	
	// Stacking
	TSet<int32> OccupiedIndices = FindOccupiedIndices(Index, GrabbedGridSpan);
	if (OccupiedIndices.IsEmpty())
	{
		OccupyGridSlotsInRange(Index, GrabbedQuery.GetGridItem().GetGridSpan());
		GrabbedQuery.Result.LastPossibleIndex = Index;
		return;
	}

	FInv_ItemSpec GrabbedItemSpec = GrabbedGridItem.GetItem()->GetItemSpec();
	TSet<FInv_GridItem*> VisitedItems {};
	for (int32 OccupiedIndex : OccupiedIndices.Array())
	{
		if (FInv_GridItem* FoundGridItem = FindOccupyingGridItem(OccupiedIndex))
		{
			if (VisitedItems.Contains(FoundGridItem)) continue;
			VisitedItems.Add(FoundGridItem);
			
			// TEST ANY ITEM
			GrabbedQuery.Result.BlockingGridItems.AddUnique(FoundGridItem);
			continue;
			// END TEST ANY ITEM
			
			FInv_ItemSpec FoundSpec = FoundGridItem->GetItem()->GetItemSpec();
			
			if (FoundSpec.GetItemTag() != GrabbedItemSpec.GetItemTag())
			{
				GrabbedQuery.Result.BlockingGridItems.AddUnique(FoundGridItem);
				continue;
			}
			
			FInv_StackFragment* FoundStackFrag = FoundSpec.GetMutableFragment<FInv_StackFragment>();
			if (!FoundStackFrag)
			{
				GrabbedQuery.Result.BlockingGridItems.AddUnique(FoundGridItem);
				continue;
			}
		}
	}
	
	for (const auto& BlockingGridItem : GrabbedQuery.Result.BlockingGridItems)
	{
		UE_LOG(LogInventory, Display, TEXT("Blocking Grid Item"));
		
		UInv_WidgetUtils::ForEach2D(GridSlots, BlockingGridItem->GetArrayIndex(), BlockingGridItem->GetGridSpan(), Columns, [](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetGridSlotState(EInv_GridSlotState::Disabled);
		});
	}
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
			GridSlot->SetArrayIndex(UInv_WidgetUtils::GridCoordinatesToIndex({Column, Row}, Columns));
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
	
	ItemWidget->OnItemClicked.AddUniqueDynamic(this, &ThisClass::OnItemClicked);
	ItemWidget->OnItemUnclicked.AddUniqueDynamic(this, &ThisClass::OnItemUnclicked);
	ItemWidget->OnItemBeginHovering.AddUniqueDynamic(this, &ThisClass::OnItemBeginHovering);
	ItemWidget->OnItemEndHovering.AddUniqueDynamic(this, &ThisClass::OnItemEndHovering);
	
	return ItemWidget;
}

void UInv_InventoryGrid::AddGridItem(const FInv_GridItem& GridItem)
{
	UInv_ItemWidget* ItemWidget = GridItem.GetItemWidget();
	const int32 Index = GridItem.GetArrayIndex();
	const FIntPoint GridSpan = GridItem.GetGridSpan();
	const FIntPoint GridCoordinates = UInv_WidgetUtils::IndexToGridCoordinates(Index, Columns);
	
	UGridSlot* ItemSlot = GridPanel_Items->AddChildToGrid(ItemWidget, GridCoordinates.Y, GridCoordinates.X);
	ItemSlot->SetColumnSpan(GridSpan.X);
	ItemSlot->SetRowSpan(GridSpan.Y);
	
	OccupyGridSlotsInRange(Index, GridSpan);
	
	if (FindGridItemByIndex(Index))
	{
		return;
	}
	
	GridItems.Add(GridItem);
}

void UInv_InventoryGrid::Stack(const FInv_SlotAvailability& SlotAvailability)
{
	FInv_GridItem* GridItem = FindMutableGridItemByIndex(SlotAvailability.ItemIndex);
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

int32 UInv_InventoryGrid::GetItemWidgetIndex(const UUserWidget* ItemWidget)
{
	const FInv_GridItem* GridItem = FindGridItemByWidget(ItemWidget);
	if (!GridItem) return INDEX_NONE;
	
	return GridItem->GetArrayIndex();
}

void UInv_InventoryGrid::OnItemClicked(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent)
{
	if (GrabbedQuery.IsGrabbing()) return;
	
	const FInv_GridItem* GridItemPtr = FindGridItemByWidget(ItemWidget);
	check(GridItemPtr != nullptr);
	
	const FInv_GridItem& GridItem = *GridItemPtr;
	const int32 ItemIndex = GridItem.GetArrayIndex();
	
	UE_LOG(LogInventory, Display, TEXT("Item %s at index %d, has been clicked."), *ItemWidget->GetName(), ItemIndex);
	
	GrabbedQuery.StartGrabbing(GridItem, UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));
	UnoccupyGridSlotsInRange(ItemIndex, GridItem.GetGridSpan());
	
	SetCursor(EMouseCursor::Type::GrabHandClosed);
}

void UInv_InventoryGrid::OnItemUnclicked(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent)
{
	if (!GrabbedQuery.IsGrabbing()) return;

	FInv_GrabbedQueryResult GrabResult = GrabbedQuery.StopGrabbing();
	
	if (GrabResult.HasFoundPossibleIndex())
	{
		const FInv_GridItem* ToRemove = FindGridItemByIndex(GrabResult.GrabbedItem.GetArrayIndex());
		
		GridItems.RemoveAll([this, ToRemove](FInv_GridItem& GridItem)
		{
			if (GridItem.GetArrayIndex() == ToRemove->GetArrayIndex())
			{
				UnoccupyGridSlotsInRange(ToRemove->GetArrayIndex(), ToRemove->GetGridSpan());
				return true;
			}
			
			return false;
		});
		
		GrabResult.GrabbedItem.SetArrayIndex(GrabResult.LastPossibleIndex);
		AddGridItem(GrabResult.GrabbedItem);
	}
	
	for (const auto& GridItem : GridItems)
	{
		if (GridItem.GetItemWidget()->IsHovered())
		{
			SetCursor(EMouseCursor::Type::GrabHand);
		}
	}
}

void UInv_InventoryGrid::OnItemBeginHovering(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent)
{
	if (GrabbedQuery.IsGrabbing()) return;
	
	SetCursor(EMouseCursor::Type::GrabHand);
}

void UInv_InventoryGrid::OnItemEndHovering(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent)
{
	if (GrabbedQuery.IsGrabbing()) return;
	
	SetCursor(EMouseCursor::Type::Default);
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
			if (const FInv_GridItem* GridItem = FindGridItemByIndex(i))
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
		
		TSet<int32> ToOccupy = FindIndecesToOccupy(i, GridSpan);
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

EInv_GridSlotQuadrant UInv_InventoryGrid::GetGridSlotQuadrant(UUserWidget* Widget) const
{
	EInv_GridSlotQuadrant Result {EInv_GridSlotQuadrant::None};
	
	const FVector2D WidgetCenter = UInv_WidgetUtils::GetWidgetCenter(Widget);
	if (!UInv_WidgetUtils::IsPositionBoundByWidget(GridPanel_Items, UInv_WidgetUtils::GetWidgetPosition(Widget))  
		&& !UInv_WidgetUtils::IsPositionBoundByWidget(GridPanel_Items, UInv_WidgetUtils::GetWidgetBottomRight(Widget)))
	{
		return Result;
	}
	
	const FVector2D GridPosition = UInv_WidgetUtils::GetWidgetPosition(GridPanel_Items);
	const float GridSlotLocalX = FMath::Fmod(WidgetCenter.X - GridPosition.X, GridSlotSize.X);
	const float GridSlotLocalY = FMath::Fmod(WidgetCenter.Y - GridPosition.Y, GridSlotSize.Y);
	
	bool bIsTop = GridSlotLocalY < GridSlotSize.Y / 2.f;
	bool bIsLeft = GridSlotLocalX < GridSlotSize.X / 2.f; 
	
	if (bIsTop && bIsLeft) Result = EInv_GridSlotQuadrant::TopLeft;
	if (bIsTop && !bIsLeft) Result = EInv_GridSlotQuadrant::TopRight;
	if (!bIsTop && bIsLeft) Result = EInv_GridSlotQuadrant::BottomLeft;
	if (!bIsTop && !bIsLeft) Result = EInv_GridSlotQuadrant::BottomRight;
	return Result;
}

FIntPoint UInv_InventoryGrid::ViewportPositionToGridCoordinate(const FVector2D& Position)
{
	const FVector2D GridTopLeft = UInv_WidgetUtils::GetWidgetPosition(GridPanel_Items);
	return {
		static_cast<int32>(FMath::FloorToInt((Position.X - GridTopLeft.X) / GridSlotSize.X)),
		static_cast<int32>(FMath::FloorToInt((Position.Y - GridTopLeft.Y) / GridSlotSize.Y))
	};
}

FIntPoint UInv_InventoryGrid::GetGridCoordinatesByQuadrant(const FVector2D& WidgetCenter, const FIntPoint& GridSpan, EInv_GridSlotQuadrant Quadrant)
{
	const FIntPoint GridCoordinates = ViewportPositionToGridCoordinate(WidgetCenter);
	if (GridSpan == FIntPoint(1)) return GridCoordinates;
	
	const int32 HasEvenWidth = GridSpan.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = GridSpan.Y % 2 == 0 ? 1 : 0;
	
	FIntPoint StartingCoordinates = GridCoordinates;
	switch (Quadrant)
	{
	case EInv_GridSlotQuadrant::None:
		break;
	case EInv_GridSlotQuadrant::TopLeft:
		StartingCoordinates.X = GridCoordinates.X - FMath::FloorToInt(0.5f * GridSpan.X);
		StartingCoordinates.Y = GridCoordinates.Y - FMath::FloorToInt(0.5f * GridSpan.Y);
		break;
	case EInv_GridSlotQuadrant::TopRight:
		StartingCoordinates.X = GridCoordinates.X - FMath::FloorToInt(0.5f * GridSpan.X) + HasEvenWidth;
		StartingCoordinates.Y = GridCoordinates.Y - FMath::FloorToInt(0.5f * GridSpan.Y);
		break;
	case EInv_GridSlotQuadrant::BottomLeft:
		StartingCoordinates.X = GridCoordinates.X - FMath::FloorToInt(0.5f * GridSpan.X);
		StartingCoordinates.Y = GridCoordinates.Y - FMath::FloorToInt(0.5f * GridSpan.Y) + HasEvenHeight;
		break;
	case EInv_GridSlotQuadrant::BottomRight:
		StartingCoordinates.X = GridCoordinates.X - FMath::FloorToInt(0.5f * GridSpan.X) + HasEvenWidth;
		StartingCoordinates.Y = GridCoordinates.Y - FMath::FloorToInt(0.5f * GridSpan.Y) + HasEvenHeight;
		break;
	}
	
	return StartingCoordinates;
}

UInv_GridSlot* UInv_InventoryGrid::FindGridSlotByIndex(int32 Index)
{
	for (auto& GridSlot: GridSlots)
	{
		if (GridSlot->GetArrayIndex() == Index) return GridSlot;
	}
	
	return nullptr;
}

UInv_GridSlot* UInv_InventoryGrid::FindGridSlotByCoordinates(const FIntPoint& GridCoordinates)
{
	const int32 Index = UInv_WidgetUtils::GridCoordinatesToIndex(GridCoordinates, Columns);
	
	for (auto& GridSlot: GridSlots)
	{
		if (GridSlot->GetArrayIndex() == Index) return GridSlot;
	}
	
	return nullptr;
}

void UInv_InventoryGrid::OccupyGridSlotsInRange(int32 Index, const FIntPoint& Range2D)
{
	UInv_WidgetUtils::ForEach2D(GridSlots, Index, Range2D, Columns, [](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetGridSlotState(EInv_GridSlotState::Occupied);
	});
}

void UInv_InventoryGrid::UnoccupyGridSlotsInRange(int32 Index, const FIntPoint& Range2D)
{
	UInv_WidgetUtils::ForEach2D(GridSlots, Index, Range2D, Columns, [](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetGridSlotState(EInv_GridSlotState::Unoccupied);
	});
}

bool UInv_InventoryGrid::CanFitRange(int32 Index, const FIntPoint& Range2D) const
{
	const FIntPoint LeftCorner = UInv_WidgetUtils::IndexToGridCoordinates(Index, Columns);
	const FIntPoint RightCorner = LeftCorner + (Range2D - FIntPoint(1,1));
	const int32 RightCornerIndex = UInv_WidgetUtils::GridCoordinatesToIndex(RightCorner, Columns);
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

TSet<int32> UInv_InventoryGrid::FindIndecesToOccupy(int32 Index, const FIntPoint& Range2D) const
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
		return Item.GetArrayIndex() == Index;
	});
	
	return Item != nullptr ? Item->GetItem() : nullptr;
}

const FInv_GridItem* UInv_InventoryGrid::FindGridItemByIndex(int32 Index) const
{
	return GridItems.FindByPredicate([Index](const FInv_GridItem& GridItem)
	{
		return GridItem.GetArrayIndex() == Index;
	});
}

FInv_GridItem* UInv_InventoryGrid::FindMutableGridItemByIndex(int32 Index)
{
	return GridItems.FindByPredicate([Index](const FInv_GridItem& GridItem)
	{
		return GridItem.GetArrayIndex() == Index;
	});
}

FInv_GridItem* UInv_InventoryGrid::FindGridItemByWidget(const UUserWidget* ItemWidget)
{
	return GridItems.FindByPredicate([ItemWidget](FInv_GridItem& GridItem)
	{
		return GridItem.GetItemWidget() == ItemWidget;
	});
}

FInv_GridItem* UInv_InventoryGrid::FindOccupyingGridItem(int32 Index)
{
	UInv_GridSlot* OccupiedSlot = FindGridSlotByIndex(Index);
	if (!OccupiedSlot || !OccupiedSlot->IsOccupied()) return nullptr;
	
	for (FInv_GridItem& GridItem : GridItems)
	{
		bool bFoundItem {false};
		UInv_WidgetUtils::ForEach2D(GridSlots, GridItem.GetArrayIndex(), GridItem.GetGridSpan(), Columns, [OccupiedSlot, &bFoundItem](UInv_GridSlot* GridSlot)
		{
			if (GridSlot == OccupiedSlot)
			{
				bFoundItem = true;
			}
		});
		
		if (bFoundItem)
		{
			return &GridItem;
		}
	}
	
	return nullptr;
}

