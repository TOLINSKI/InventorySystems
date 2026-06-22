// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Inventory.h"
#include "Blueprint/WidgetTree.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Inv_InventoryComponent.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Equipment/Inv_EquipmentProxyWidget.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/Item/Inv_ItemWidget.h"

void UInv_SpatialInventory::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetIsFocusable(true);
}

void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Grid_Equippables->OnGridBeginGrabItem.BindUObject(this, &UInv_SpatialInventory::OnGridBeginGrabItem);
	Grid_Consumables->OnGridBeginGrabItem.BindUObject(this, &UInv_SpatialInventory::OnGridBeginGrabItem);
	Grid_Craftables->OnGridBeginGrabItem.BindUObject(this, &UInv_SpatialInventory::OnGridBeginGrabItem);
	
	Grid_Equippables->OnGridEndGrabItem.BindUObject(this, &UInv_SpatialInventory::OnGridEndGrabItem);
	Grid_Consumables->OnGridEndGrabItem.BindUObject(this, &UInv_SpatialInventory::OnGridEndGrabItem);
	Grid_Craftables->OnGridEndGrabItem.BindUObject(this, &UInv_SpatialInventory::OnGridEndGrabItem);
	
	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UInv_EquippedGridSlot* EquipSlot = Cast<UInv_EquippedGridSlot>(Widget))
		{
			EquipSlot->OnEquipSlotClicked.AddDynamic(this, &ThisClass::OnEquipSlotPressed);
			EquipSlot->OnEquipSlotUnClicked.AddDynamic(this, &ThisClass::OnEquipSlotUnPressed);
			EquipSlot->OnGridSlotBeginHover.AddDynamic(this, &ThisClass::OnEquipSlotBeginHover);
			EquipSlot->OnGridSlotEndHover.AddDynamic(this, &ThisClass::OnEquipSlotEndHover);
		}
		
		if (UInv_EquipmentProxyWidget* ProxyWidget = Cast<UInv_EquipmentProxyWidget>(Widget))
		{
			ProxyWidget->OnProxyPressed.BindUObject(this, &ThisClass::OnProxyPressed);
			ProxyWidget->OnProxyUnPressed.BindUObject(this, &ThisClass::OnProxyUnPressed);
			ProxyWidget->OnProxyEndHover.BindWeakLambda(this, [](UInv_EquipmentProxyWidget* Widget)
			{
				Widget->StopDragging();
			});
		}
	});
	
	SwitchGridCategory(EInv_ItemCategory::Equippable);
}

void UInv_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (GrabbedGridItem && ShouldDropGrabbedItem())
	{
		if (!bHasChangedGrabbedItemGridPossibilities)
		{
			bHasChangedGrabbedItemGridPossibilities = true;
			GetActiveGrid()->HidePossibleGridSlotsForGrabbedItem();
		}
	}
	else if (GrabbedGridItem)
	{
		if (bHasChangedGrabbedItemGridPossibilities)
		{
			GetActiveGrid()->ShowPossibleGridSlotsForGrabbedItem();
			bHasChangedGrabbedItemGridPossibilities = false;
		}
	}
	else
	{
		bHasChangedGrabbedItemGridPossibilities = false;
	}
}

FReply UInv_SpatialInventory::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (!GrabbedGridItem) return FReply::Unhandled();
	
		if (ShouldDropGrabbedItem())
			DropGrabbedItem();
		else
			PlaceGrabbedItemBackOnGrid();
		
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

void UInv_SpatialInventory::OnGridBeginGrabItem(FInv_GridItem& GridItem)
{
	GrabbedGridItem = &GridItem;
	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
	SetFocus();
}

void UInv_SpatialInventory::OnGridEndGrabItem(bool bItemPlacedOnGrid)
{
	if (WasItemPreviouslyEquipped(GrabbedGridItem))
	{
		if (bItemPlacedOnGrid) 
			UnequipGridItem(GrabbedGridItem);
		else 
			EquipGridItem(GrabbedGridItem, GrabbedGridItem->GetEquippedGridSlot());
	}
	
	GrabbedGridItem = nullptr;
	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
	SetCursor(EMouseCursor::Default);
}

void UInv_SpatialInventory::OnEquipSlotPressed(UInv_EquippedGridSlot* EquipSlot, const FGameplayTag& EquipmentTag)
{
	if (!EquipSlot->GetGridItem()) return;
	
	FInv_GridItem* EquippedItem = EquipSlot->UnequipItem();
	GetActiveGrid()->StartGrabbing(EquippedItem);	
	SetCursor(EMouseCursor::GrabHandClosed);
}

void UInv_SpatialInventory::OnEquipSlotUnPressed(UInv_EquippedGridSlot* EquipSlot, const FGameplayTag& EquipmentTag)
{
	if (!GrabbedGridItem) return;
	
	if (!CanEquipGrabbedItem(EquipSlot))
	{
		PlaceGrabbedItemBackOnGrid();
		return;
	}

	EquipGridItem(GrabbedGridItem, EquipSlot);
	
	SetCursor(EMouseCursor::GrabHand);
}

void UInv_SpatialInventory::OnEquipSlotBeginHover(UInv_GridSlot* EquipSlot)
{
	if (EquipSlot->GetGridItem())
	{
		SetCursor(EMouseCursor::GrabHand);
		EquipSlot->SetGridSlotAvailability(EInv_GridSlotState::Selected);
		GetActiveGrid()->SetDescriptionPopUpTimer(EquipSlot->GetGridItem());
	}
	else if (CanEquipGrabbedItem(Cast<UInv_EquippedGridSlot>(EquipSlot)))
	{
		EquipSlot->SetGridSlotAvailability(EInv_GridSlotState::Selected);
	}
}

void UInv_SpatialInventory::OnEquipSlotEndHover(UInv_GridSlot* EquipSlot)
{
	if (!EquipSlot->GetGridItem())
	{
		EquipSlot->SetGridSlotAvailability(EInv_GridSlotState::Unoccupied);
	}
	else
	{
		EquipSlot->SetGridSlotAvailability(EInv_GridSlotState::Occupied);
		SetCursor(EMouseCursor::Default);
		GetActiveGrid()->HidePopUpDescription();
	}
}

void UInv_SpatialInventory::OnProxyUnPressed(UInv_EquipmentProxyWidget* ProxyWidget, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton) return; 
	
	if (GrabbedGridItem && GrabbedGridItem->IsValid())
	{
		PlaceGrabbedItemBackOnGrid();
	}
	
	ProxyWidget->StopDragging();
}

void UInv_SpatialInventory::OnProxyPressed(UInv_EquipmentProxyWidget* ProxyWidget, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton) return; 
	
	if (GrabbedGridItem && GrabbedGridItem->IsValid())
	{
		PlaceGrabbedItemBackOnGrid();
		return;
	}
	
	ProxyWidget->StartDragging();
}

void UInv_SpatialInventory::EquipGridItem(FInv_GridItem* GridItem, UInv_EquippedGridSlot* EquipSlot)
{
	if (WasItemPreviouslyEquipped(GridItem))
	{
		check(GridItem && GridItem->IsValid());
	
		GridItem->SetArrayIndex(INDEX_NONE);
		EquipSlot->EquipGridItem(*GridItem);
		GetActiveGrid()->StopGrabbing();
		GridItem = nullptr;
		return;
	}
	
	// Copy grid item and remove from original grid
	FInv_GridItem& EquippedGridItem = EquippedGridItems.Add_GetRef(*GridItem);
	UInv_InventoryGrid* ActiveGrid = GetActiveGrid();
	ActiveGrid->StopGrabbing();
	ActiveGrid->RemoveGridItem(*GridItem);
	ActiveGrid->SetCursor(EMouseCursor::Default);
	GridItem = nullptr;
		
	// Equip item
	EquippedGridItem.SetArrayIndex(INDEX_NONE);
	EquipSlot->EquipGridItem(EquippedGridItem);
	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
	
	UInv_InventoryComponent* IC = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	IC->Server_EquipItem(EquippedGridItem.GetItem());
}

void UInv_SpatialInventory::UnequipGridItem(FInv_GridItem* GridItem)
{
	UInv_EquippedGridSlot* EquipSlot = GridItem->GetEquippedGridSlot();
	check(EquipSlot != nullptr);
	
	const int32 EquipIndex = EquippedGridItems.IndexOfByPredicate([EquipSlot](const FInv_GridItem& GridItem)
	{
		return GridItem.GetEquippedGridSlot() == EquipSlot;
	});
	
	UInv_InventoryComponent* IC = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	IC->Server_UnequipItem(GridItem->GetItem());

	GridItem->SetEquipSlot(nullptr);
	
	if (!EquippedGridItems.IsValidIndex(EquipIndex)) return;
	EquippedGridItems.RemoveAtSwap(EquipIndex);
}

bool UInv_SpatialInventory::CanEquipGrabbedItem(const UInv_EquippedGridSlot* EquippedSlot) const
{
	return	IsValid(EquippedSlot)
			&& !EquippedSlot->IsOccupied() 
			&& GrabbedGridItem != nullptr
			&& GrabbedGridItem->IsValid()
			&& GrabbedGridItem->GetItemTag().MatchesTag(EquippedSlot->GetEquipmentTag());
}

void UInv_SpatialInventory::PlaceGrabbedItemBackOnGrid()
{
	check(GrabbedGridItem != nullptr);
	OnGridEndGrabItem(GetActiveGrid()->TryPlaceGrabbedItemOnGrid());
}

bool UInv_SpatialInventory::WasItemPreviouslyEquipped(FInv_GridItem* GridItem)
{
	return GrabbedGridItem->GetEquippedGridSlot() != nullptr;
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::GetGridAvailability(UInv_ItemComponent* ItemComponent) const
{
	const UInv_InventoryGrid* GridToCheck = GetGridByCategory(ItemComponent->GetItemSpec().GetItemCategory());
	check(IsValid(GridToCheck));
	return GridToCheck->SearchGridForSlotAvailability(ItemComponent);
}

void UInv_SpatialInventory::SwitchGridCategory(EInv_ItemCategory Category)
{
	auto GridToActivate = GetGridByCategory(Category);
	Grid_Switcher->SetActiveWidget(GridToActivate);
	
	OnCategorySelected(Category);
}

UInv_InventoryItem* UInv_SpatialInventory::GetGrabbedItem() const
{
	if (const UInv_InventoryGrid* ActiveGrid = GetActiveGrid())
	{
		return ActiveGrid->GetGrabbedItem();
	}
	
	return nullptr;
}

UUserWidget* UInv_SpatialInventory::GetGrabbedWidget() const
{
	return GrabbedGridItem ? GrabbedGridItem->GetItemWidget() : nullptr;
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

void UInv_SpatialInventory::DropGrabbedItem()
{
	if (WasItemPreviouslyEquipped(GrabbedGridItem))
	{
		UInv_InventoryComponent* IC = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
		IC->Server_UnequipItem(GrabbedGridItem->GetItem());
	}
	
	GetActiveGrid()->DropGrabbedItem();
	
	GrabbedGridItem = nullptr;
	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
	SetCursor(EMouseCursor::Default);
}
