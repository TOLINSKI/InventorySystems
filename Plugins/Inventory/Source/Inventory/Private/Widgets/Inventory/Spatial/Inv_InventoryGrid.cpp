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
#include "Widgets/Item/Inv_ItemDescription.h"
#include "Widgets/Item/Inv_ItemPopUp.h"
#include "Widgets/Item/Inv_ItemWidget.h"
#include "Widgets/BxWidgetUtils.h"

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
	
	if (GrabbedQuery.IsGrabbing())
	{
		UpdateGrabbedQuery();
	}
}

void UInv_InventoryGrid::ReverseLastTickGridSlotEffects()
{
	// Unoccupy last tick possible grid slots
	if (FInv_GridItem* GrabbedGridItem = GrabbedQuery.GetGridItem(); GrabbedQuery.HasFoundPossibleGridIndex())
	{
		SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Unoccupied, GrabbedQuery.GetPossibleIndex(), GrabbedGridItem->GetGridSpan());
	}
	
	// Occupy last tick blocking grid slots
	for (const auto& BlockingGridItem : GrabbedQuery.GetBlockingGridItems())
	{
		SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Occupied, BlockingGridItem->GetIndex(), BlockingGridItem->GetGridSpan());
	}
	GrabbedQuery.GetBlockingGridItems().Empty();

	// Occupy last tick stackable grid slots
	if (FInv_GridItem* LastStackable = GrabbedQuery.GetStackableGridItem())
	{
		SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Occupied, LastStackable->GetIndex(), LastStackable->GetGridSpan());
	}
}

void UInv_InventoryGrid::UpdateGrabbedQuery()
{
	ReverseLastTickGridSlotEffects();
	
	FInv_GridItem* GrabbedGridItem = GrabbedQuery.GetGridItem();
	
	// Positioning
	GrabbedQuery.UpdateGrabbedItemPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));
	const EInv_GridSlotQuadrant GridSlotQuadrant = GetGridSlotQuadrant(GrabbedGridItem->GetItemWidget());

	// UE_LOG(LogInventory, Display, TEXT("Grid Slot Quadrant: %s"), *UEnum::GetValueAsString(GridSlotQuadrant));
	
	// Item is held outside the grid, return
	if (GridSlotQuadrant == EInv_GridSlotQuadrant::None)
	{
		if (GrabbedQuery.HasFoundPossibleGridIndex())
		{
			SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Occupied, GrabbedQuery.GetPossibleIndex(), GrabbedGridItem->GetGridSpan());
		}
		return;
	}
	
	// Grid coordinates
	const FIntPoint GrabbedGridSpan = GrabbedGridItem->GetGridSpan();
	const FVector2D GrabbedCenter = UBxWidgetUtils::GetCachedWidgetCenter(GrabbedGridItem->GetItemWidget());
	const FIntPoint GridCoordinates = GetGridCoordinatesByQuadrant(GrabbedCenter, GrabbedGridSpan, GridSlotQuadrant);
	
	// Part of the item slides out of the grid, return
	const int32 Index = UInv_WidgetUtils::GridCoordinatesToIndex(GridCoordinates, Columns);
	if (!CanFitRange(Index, GrabbedGridSpan)) return;

	// No collisions? set possible index and return
	TSet<int32> OccupiedIndices = FindOccupiedIndices(Index, GrabbedGridSpan);
	if (OccupiedIndices.IsEmpty())
	{
		SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Occupied, Index, GrabbedQuery.GetGridItem()->GetGridSpan());
		GrabbedQuery.SetPossibleIndex(Index);
		return;
	}

	// Stacking - Check if occupied indices belong to a stackable item
	TSet<FInv_GridItem*> VisitedItems {};
	for (int32 OccupiedIndex : OccupiedIndices.Array())
	{
		if (FInv_GridItem* OverlappingGridItem = FindOverlappingGridItem(OccupiedIndex))
		{
			if (VisitedItems.Contains(OverlappingGridItem)) continue;
			VisitedItems.Add(OverlappingGridItem);
			
			// Grabbed item not stackable - Add blocking item
			if (!GrabbedGridItem->IsStackable())
			{
				GrabbedQuery.GetBlockingGridItems().AddUnique(OverlappingGridItem);
				continue;
			}
			
			// Check that overlapping item has same item tag
			if (OverlappingGridItem->GetItemTag() != GrabbedGridItem->GetItemTag())
			{
				GrabbedQuery.GetBlockingGridItems().AddUnique(OverlappingGridItem);
				continue;
			}
			
			// Check that overlapping item has place to stack
			if (!OverlappingGridItem->HasRoomToStack())
			{
				GrabbedQuery.GetBlockingGridItems().AddUnique(OverlappingGridItem);
				continue;
			}
			
			// Set overlapping item as stackable
			GrabbedQuery.SetStackableGridItem(OverlappingGridItem);
			UInv_WidgetUtils::ForEach2D(GridSlots, OverlappingGridItem->GetIndex(), OverlappingGridItem->GetGridSpan(), Columns, [](UInv_GridSlot* GridSlot)
			{
				GridSlot->SetGridSlotAvailability(EInv_GridSlotState::Selected);
			});
		}
	}
	
	for (const auto& BlockingGridItem : GrabbedQuery.GetBlockingGridItems())
	{
		UInv_WidgetUtils::ForEach2D(GridSlots, BlockingGridItem->GetIndex(), BlockingGridItem->GetGridSpan(), Columns, [](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetGridSlotAvailability(EInv_GridSlotState::Disabled);
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
			GridSlot->OnGridSlotPressed.AddDynamic(this, &ThisClass::OnGridSlotPressed);
			GridSlot->OnGridSlotBeginHover.AddDynamic(this, &ThisClass::OnGridSlotBeginHover); // ?
			GridSlot->OnGridSlotEndHover.AddDynamic(this, &ThisClass::OnGridSlotEndHover); // ?
		}
	}
}

UInv_InventoryItem* UInv_InventoryGrid::GetGrabbedItem() const
{
	return GrabbedQuery.IsGrabbing() ? GrabbedQuery.GetGridItem()->GetItem() : nullptr;
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
	Brush.ImageSize = SlotSize * GridFragment->GetGridSpan();
	ItemWidget->GetIconImage()->SetBrush(Brush);
	
	return ItemWidget;
}

void UInv_InventoryGrid::SetGridItemOnSlots(FInv_GridItem& GridItem)
{
	UInv_ItemWidget* ItemWidget = GridItem.GetItemWidget();
	ItemWidget->SetVisibility(ESlateVisibility::Visible);
	const int32 Index = GridItem.GetIndex();
	const FIntPoint GridSpan = GridItem.GetGridSpan();
	const FIntPoint GridCoordinates = UInv_WidgetUtils::IndexToGridCoordinates(Index, Columns);
	
	UGridSlot* ItemSlot = GridPanel_Items->AddChildToGrid(ItemWidget, GridCoordinates.Y, GridCoordinates.X);
	ItemSlot->SetColumnSpan(GridSpan.X);
	ItemSlot->SetRowSpan(GridSpan.Y);
	
	UInv_WidgetUtils::ForEach2D(GridSlots, GridItem.GetIndex(), GridItem.GetGridSpan(), Columns, [GridItemPtr = &GridItem](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetGridSlotAvailability(EInv_GridSlotState::Occupied);
		GridSlot->SetGridItem(GridItemPtr);
	});
}

void UInv_InventoryGrid::ResetGridItemFromSlots(FInv_GridItem& GridItem)
{
	UInv_WidgetUtils::ForEach2D(GridSlots, GridItem.GetIndex(), GridItem.GetGridSpan(), Columns, [GridItemPtr = &GridItem](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetGridSlotAvailability(EInv_GridSlotState::Unoccupied);
		GridSlot->ResetGridItem();
	});
}

void UInv_InventoryGrid::Stack(const FInv_SlotAvailability& SlotAvailability)
{
	FInv_GridItem* GridItem = FindGridItem(SlotAvailability.ItemIndex);
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

		const FInv_StackFragment* StackFragment = ItemSpec.GetFragment<FInv_StackFragment>();
		const int32 MaxStackCount = StackFragment ? StackFragment->GetMaxStackCount() : 1 ;
		
		UInv_ItemWidget* ItemWidget = CreateItemWidget(GridFragment, IconFragment);
		
		FInv_GridItem& GridItem = GridItems.Add_GetRef(FInv_GridItem(Item, ItemWidget, SlotAvailability.ItemIndex, GridFragment->GetGridSpan(), SlotAvailability.StackCount, MaxStackCount));
		SetGridItemOnSlots(GridItem);
		
		if (StackFragment)
		{
			ItemWidget->StackCountChanged(SlotAvailability.StackCount);
		}
	}
}

int32 UInv_InventoryGrid::GetItemWidgetIndex(const UUserWidget* ItemWidget)
{
	const FInv_GridItem* GridItem = FindGridItem(ItemWidget);
	if (!GridItem) return INDEX_NONE;
	
	return GridItem->GetIndex();
}

void UInv_InventoryGrid::RemoveGridItem(const FInv_GridItem& GridItem)
{
	if (GrabbedQuery.IsGrabbing() && GrabbedQuery.GetGridItem() && *GrabbedQuery.GetGridItem() == GridItem) StopGrabbing();
	
	GridItem.GetItemWidget()->OnItemBeginHover.RemoveAll(this);
	GridItem.GetItemWidget()->OnItemEndHover.RemoveAll(this);
	
	SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Unoccupied, GridItem.GetIndex(), GridItem.GetGridSpan());
	
	
	
	const int32 IndexToRemove = GridItems.IndexOfByKey(GridItem);
	check(GridItems.IsValidIndex(IndexToRemove));
	GridItems.RemoveAtSwap(IndexToRemove);
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
		
		TSet<int32> ToOccupy = FindIndicesToOccupy(i, GridSpan);
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
	
	const FVector2D WidgetCenter = UBxWidgetUtils::GetCachedWidgetCenter(Widget);
	if (!UBxWidgetUtils::IsPositionBoundByCachedWidget(UBxWidgetUtils::GetCachedWidgetPosition(Widget), GridPanel_Items)  
		&& !UBxWidgetUtils::IsPositionBoundByCachedWidget(UBxWidgetUtils::GetCachedWidgetBottomRight(Widget), GridPanel_Items))
	{
		return Result;
	}
	
	const FVector2D GridPosition = UBxWidgetUtils::GetCachedWidgetPosition(GridPanel_Items);
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
	const FVector2D GridTopLeft = UBxWidgetUtils::GetCachedWidgetPosition(GridPanel_Items);
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
		StartingCoordinates.X = -1.f;
		StartingCoordinates.Y = -1.f;
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

void UInv_InventoryGrid::SetGridSlotsAvailabilityInRange(EInv_GridSlotState State, int32 Index, const FIntPoint& Range2D)
{
	UInv_WidgetUtils::ForEach2D(GridSlots, Index, Range2D, Columns, [State](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetGridSlotAvailability(State);
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

TSet<int32> UInv_InventoryGrid::FindIndicesToOccupy(int32 Index, const FIntPoint& Range2D) const
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

const FInv_GridItem* UInv_InventoryGrid::FindGridItemByIndex(int32 Index) const
{
	return GridItems.FindByPredicate([Index](const FInv_GridItem& GridItem)
	{
		return GridItem.GetIndex() == Index;
	});
}

FInv_GridItem* UInv_InventoryGrid::FindGridItem(int32 Index)
{
	return GridItems.FindByPredicate([Index](const FInv_GridItem& GridItem)
	{
		return GridItem.GetIndex() == Index;
	});
}

FInv_GridItem* UInv_InventoryGrid::FindGridItem(const UUserWidget* ItemWidget)
{
	return GridItems.FindByPredicate([ItemWidget](FInv_GridItem& GridItem)
	{
		return GridItem.GetItemWidget() == ItemWidget;
	});
}

FInv_GridItem* UInv_InventoryGrid::FindOverlappingGridItem(int32 Index)
{
	UInv_GridSlot* OccupiedSlot = FindGridSlotByIndex(Index);
	if (!OccupiedSlot || !OccupiedSlot->IsOccupied()) return nullptr;
	
	for (FInv_GridItem& GridItem : GridItems)
	{
		bool bFoundItem {false};
		UInv_WidgetUtils::ForEach2D(GridSlots, GridItem.GetIndex(), GridItem.GetGridSpan(), Columns, [OccupiedSlot, &bFoundItem](UInv_GridSlot* GridSlot)
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

FInv_GridItem* UInv_InventoryGrid::FindGridItem(UInv_GridSlot* GridSlot)
{
	const int32 ArrayIndex = GridSlot->GetArrayIndex();
	return FindOverlappingGridItem(ArrayIndex);
}

void UInv_InventoryGrid::CreatePopUpMenu(FInv_GridItem& GridItem)
{
	if (!IsValid(PopUpMenu.GetWidget()))
	{
		check(PopUpMenuClass != nullptr);
		UInv_ItemPopUp* PopUpWidget = CreateWidget<UInv_ItemPopUp>(this, PopUpMenuClass);
		PopUpWidget->OnSplitItem.AddDynamic(this, &ThisClass::UInv_InventoryGrid::OnPopUpMenuSplitAction);
		PopUpWidget->OnUseItem.AddDynamic(this, &ThisClass::UInv_InventoryGrid::OnPopUpMenuUseAction);
		PopUpWidget->OnDropItem.AddDynamic(this, &ThisClass::UInv_InventoryGrid::OnPopUpMenuDropAction);
		PopUpMenu.Init(GridItem, PopUpWidget);
	}
	else
	{
		PopUpMenu.Init(GridItem);
	}

	UInv_ItemPopUp* PopUpWidget = Cast<UInv_ItemPopUp>(PopUpMenu.GetWidget());
	// Handle Category
	PopUpWidget->SetItemCategory(GridItem.GetItem()->GetItemSpec().GetItemCategory());
	
	// Handle Stack
	if (GridItem.GetStackCount() > 1)
	{
		PopUpWidget->SetMaxValue(GridItem.GetStackCount());
		PopUpWidget->SetValueEnabled(true);
	}
	else
	{
		PopUpWidget->SetMaxValue(1.f);
		PopUpWidget->SetValueEnabled(false);
	}
	PopUpWidget->SetValue(1.f);
	
	// Handle Visibility
	PopUpWidget->AddToViewport();
	PopUpWidget->SetVisibility(ESlateVisibility::Visible);
	
	// Handle Position
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	PopUpWidget->SetPositionInViewport(MousePosition + FVector2D(-20.f, -20.f), false);
}

void UInv_InventoryGrid::StartGrabbing(FInv_GridItem* GridItem)
{
	if (GrabbedQuery.IsGrabbing()) return;
	
	check(GridItem != nullptr);
	UInv_ItemWidget* ItemWidget = GridItem->GetItemWidget(); 
	check(ItemWidget != nullptr);
	
	// Handle grabbed widget
	const int32 ItemIndex = GridItem->GetIndex();
	ItemWidget->RemoveFromParent();
	ItemWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	ItemWidget->AddToViewport();
	
	// Handle grid slot availability
	if (ItemIndex != INDEX_NONE)
	{
		SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Unoccupied, ItemIndex, GridItem->GetGridSpan());
	}
	
	// Handle Grab Query
	UInv_GridSlot* GridSlot { nullptr };
	if (GridSlots.IsValidIndex(ItemIndex))
	{
		GridSlot = GridSlots[ItemIndex];
	}
	GrabbedQuery.StartGrabbing(*GridItem, UBxWidgetUtils::GetCachedWidgetPosition(GridSlot), UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));
		
	SetCursor(EMouseCursor::Type::GrabHandClosed);
	
	OnGridBeginGrabItem.ExecuteIfBound(*GridItem);
}
 
void UInv_InventoryGrid::StartGrabbing(const FInv_GridItem& GridItem)
{
	// if (GrabbedQuery.IsGrabbing()) return;
	//
	// const FInv_ItemSpec& ItemSpec = GridItem.GetItem()->GetItemSpec();
	// const FInv_GridFragment* GridFragment = ItemSpec.GetFragment<FInv_GridFragment>();
	// const FInv_IconFragment* IconFragment = ItemSpec.GetFragment<FInv_IconFragment>();
	// check(GridFragment);
	// check(IconFragment);
	//
	// UInv_ItemWidget* ItemWidget = CreateItemWidget(GridFragment, IconFragment);
	// ItemWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	//
	// const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	// ItemWidget->AddToViewport();
	// ItemWidget->SetPositionInViewport(MousePosition - FVector2D(20.f), false);
	//
	// FInv_GridItem& NewGridItem = GridItems.Add_GetRef(FInv_GridItem(GridItem.GetItem(), ItemWidget, INDEX_NONE, GridFragment->GetGridSpan(), GridItem.GetStackCount(), GridItem.GetMaxStackCount()));
	// GrabbedQuery.StartGrabbing(NewGridItem, MousePosition);
}

void UInv_InventoryGrid::OnGridSlotPressed(UInv_GridSlot* GridSlot, const FPointerEvent& MouseEvent)
{
	HidePopUpDescription();
	
	FInv_GridItem* GridItem = GridSlot->GetGridItem();
	if (!GridItem) return;
	
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		StartGrabbing(GridItem);
	}
	
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		CreatePopUpMenu(*GridItem);
	}
}

void UInv_InventoryGrid::ExchangeStacks(FInv_GridItem& Source, FInv_GridItem& TargetItem)
{
	const int32 AvailableStackableAmount = TargetItem.GetStackableAmount();
	const int32 DesiredStackAmount = Source.GetStackCount();
	const int32 ActualStackAmount = FMath::Min(AvailableStackableAmount, DesiredStackAmount);
		
	TargetItem.AddStackCount(ActualStackAmount);
	Source.SubtractStackCount(ActualStackAmount);
}

void UInv_InventoryGrid::PlaceGrabbedItemOnGrid()
{
	if (!GrabbedQuery.IsGrabbing()) return;
	GrabbedQuery.StopGrabbing();
	
	ReverseLastTickGridSlotEffects();
	FInv_GridItem* GrabbedGridItem = GrabbedQuery.GetGridItem();
	
	// STACKED: Add all possible stacks. If got rid of all stacks, remove the item from the grid.
	if (FInv_GridItem* StackableGridItem = GrabbedQuery.GetStackableGridItem())
	{
		ExchangeStacks(*GrabbedGridItem, *StackableGridItem);
		if (GrabbedGridItem->GetStackCount() == 0)
		{
			RemoveGridItem(*GrabbedGridItem);
			GrabbedGridItem = nullptr;
		}
	}
	
	// Place item back on grid if needed
	if (GrabbedGridItem && GrabbedQuery.HasFoundPossibleGridIndex())
	{
		GrabbedGridItem->ResetArrayIndex(GrabbedQuery.GetPossibleIndex());
		SetGridItemOnSlots(*GrabbedGridItem);
	}	
	
	// Set cursor as grab hand if needed
	for (const auto& GridItem : GridItems)
	{
		if (GridItem.GetItemWidget()->IsHovered())
		{
			SetCursor(EMouseCursor::Type::GrabHand);
		}
	}
}

void UInv_InventoryGrid::StopGrabbing()
{
	if (!GrabbedQuery.IsGrabbing()) return;
	GrabbedQuery.StopGrabbing();
}

void UInv_InventoryGrid::DropGrabbedItem()
{
	if (!GrabbedQuery.IsGrabbing()) return;
	GrabbedQuery.StopGrabbing();
	
	DropGridItem(GrabbedQuery.GetGridItem());
}

void UInv_InventoryGrid::HidePopUpDescription()
{
	if (UUserWidget* DescriptionWidget = PopUpDescription.GetWidget())
	{
		DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
		DescriptionWidget->RemoveFromParent();
	}
	
	GetWorld()->GetTimerManager().ClearTimer(DescriptionTimer);
}

void UInv_InventoryGrid::OnGridSlotBeginHover(UInv_GridSlot* GridSlot)
{
	if (GrabbedQuery.IsGrabbing()) return;
	
	FInv_GridItem* GridItem = FindGridItem(GridSlot);
	if(!GridItem) return;
	
	SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Selected, GridItem->GetIndex(), GridItem->GetGridSpan());
	GetWorld()->GetTimerManager().SetTimer(DescriptionTimer, [this, GridItem]()
	{
	   CreatePopUpDescription(*GridItem);
	}, ShowDescriptionDelay, false);

	SetCursor(EMouseCursor::Type::GrabHand);
}

void UInv_InventoryGrid::OnGridSlotEndHover(UInv_GridSlot* GridSlot)
{
	if (GrabbedQuery.IsGrabbing()) return;
	
	const FInv_GridItem* LastGridItem = GridSlot->GetGridItem();
	if (!LastGridItem) return;
	
	if (UInv_GridSlot* NewGridSlot = GetHoveredGridSlot())
	{
		const FInv_GridItem* NewGridItem = NewGridSlot->GetGridItem();
		if (NewGridItem != LastGridItem)
		{
			HidePopUpDescription();
			SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Occupied, LastGridItem->GetIndex(), LastGridItem->GetGridSpan());
			
			if (!NewGridItem)
			{
				// We are not pointing to any items anymore
				SetCursor(EMouseCursor::Type::Default);
				return;
			}
		}
	}

	// Not hovering grid slots anymore, set default cursor
	HidePopUpDescription();
	SetGridSlotsAvailabilityInRange(EInv_GridSlotState::Occupied, LastGridItem->GetIndex(), LastGridItem->GetGridSpan());
	SetCursor(EMouseCursor::Type::Default);
}

void UInv_InventoryGrid::OnItemPressed(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent)
{
	
}

void UInv_InventoryGrid::OnItemBeginHover(UInv_ItemWidget* ItemWidget)
{
}

void UInv_InventoryGrid::OnItemEndHover(UInv_ItemWidget* ItemWidget)
{
}

void UInv_InventoryGrid::OnPopUpMenuSplitAction(int32 SplitAmount)
{
	PopUpMenu.GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
	FInv_GridItem* GridItem = PopUpMenu.GetGridItem();
	
	// TODO ? Remove this?
}

void UInv_InventoryGrid::OnPopUpMenuUseAction(int32 UsedAmount)
{
	PopUpMenu.GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
	
	FInv_GridItem* GridItem = PopUpMenu.GetGridItem();
	UseGridItem(GridItem, 1);
}

void UInv_InventoryGrid::OnPopUpMenuDropAction(int32 DropAmount)
{
	PopUpMenu.GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
	
	FInv_GridItem* GridItem = PopUpMenu.GetGridItem();
	DropGridItem(GridItem, DropAmount);
}

void UInv_InventoryGrid::DropGridItem(FInv_GridItem* GridItem, int32 Amount)
{
	if (Amount <= 0)
	{
		Amount = GridItem->GetStackCount();
	}

	// Handle multiplayer (Inventory Component)
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));
	InventoryComponent->Server_DropItem(GridItem->GetItem(), Amount, true);
	
	// Handle UI
	ReverseLastTickGridSlotEffects();
	if (GridItem->SubtractStackCount(Amount) <= 0)
	{
		RemoveGridItem(*GridItem);	
	}
}

void UInv_InventoryGrid::UseGridItem(FInv_GridItem* GridItem, int32 Amount)
{
	// Handle multiplayer (Inventory Component)
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));
	InventoryComponent->Server_UseItem(GridItem->GetItem(), Amount);
	
	// Handle UI
	if (GridItem->SubtractStackCount(Amount) <= 0)
	{
		RemoveGridItem(*GridItem);
	}
}

void UInv_InventoryGrid::CreatePopUpDescription(FInv_GridItem& GridItem)
{
	if (!IsValid(PopUpDescription.GetWidget()))
	{
		check(PopUpDescriptionClass != nullptr);
		UInv_ItemDescription* PopUpWidget = CreateWidget<UInv_ItemDescription>(this, PopUpDescriptionClass);
		PopUpDescription.Init(GridItem, PopUpWidget);
	}
	else
	{
		PopUpDescription.Init(GridItem);
	}
	   
	const FInv_ItemSpec& ItemSpec = GridItem.GetItem()->GetItemSpec();
	UInv_CompositeWidget* DescriptionWidget = Cast<UInv_CompositeWidget>(PopUpDescription.GetWidget());
	DescriptionWidget->ApplyFunction([ItemSpec](UInv_CompositeWidget* CompositeWidget)
	{
		ItemSpec.ApplyToCompositeWidget(CompositeWidget);
	});
	
	// Handle Visibility
	DescriptionWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	DescriptionWidget->AddToViewport();
	DescriptionWidget->ForceLayoutPrepass();
	
	// Handle Position
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	const FVector2D ClampedWidgetPosition = 
		UInv_WidgetUtils::GetClampedWidgetPosition(
			DescriptionWidget, 
			MousePosition + FVector2D(-20.f, -20.f));
	
	DescriptionWidget->SetPositionInViewport(ClampedWidgetPosition, false);
}

UInv_GridSlot* UInv_InventoryGrid::GetHoveredGridSlot() const
{
	for (const auto& GridSlot : GridSlots)
	{
		if (GridSlot->IsHovered())
		{
			return GridSlot;
		}
	}
	
	return nullptr;
}

