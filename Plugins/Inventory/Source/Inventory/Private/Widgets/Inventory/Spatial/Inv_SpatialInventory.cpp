// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Inventory.h"
#include "Blueprint/WidgetTree.h"
#include "Components/WidgetSwitcher.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Inv_ItemTags.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Widgets/BxWidgetUtils.h"
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
	
	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UInv_EquippedGridSlot* EquipSlot = Cast<UInv_EquippedGridSlot>(Widget))
		{
			EquipSlot->OnEquipSlotClicked.AddDynamic(this, &ThisClass::OnEquipSlotClicked);
			EquipSlot->OnEquipSlotUnClicked.AddDynamic(this, &ThisClass::OnEquipSlotUnClicked);
		}
	});
	
	SwitchGridCategory(EInv_ItemCategory::Equippable);
}

FReply UInv_SpatialInventory::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (!GrabbedGridItem)
		{
			return FReply::Unhandled();
		}
	
		if (ShouldDropItem())
		{
			// Drop the item (do not return it to the grid)
			GetActiveGrid()->DropGrabbedItem();
		}
		else
		{
			// Handle returning or stacking the item in the inventory
			GetActiveGrid()->PlaceGrabbedItemOnGrid();
		}
		
		GrabbedGridItem = nullptr;
		GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
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

void UInv_SpatialInventory::OnEquipSlotClicked(UInv_EquippedGridSlot* EquipSlot, const FGameplayTag& EquipmentTag)
{
	// ???
}

void UInv_SpatialInventory::OnEquipSlotUnClicked(UInv_EquippedGridSlot* EquipSlot, const FGameplayTag& EquipmentTag)
{
	if (!CanEquipGrabbedItem(EquipSlot)) { return; }

	// Copy grid item and remove from original grid
	FInv_GridItem& EquippedGridItem = EquippedGridItems.Add_GetRef(*GrabbedGridItem);
	GetActiveGrid()->StopGrabbing();
	GetActiveGrid()->RemoveGridItem(*GrabbedGridItem);
	GrabbedGridItem = nullptr;
	
	// Handle newly copied grid item
	EquippedGridItem.ResetArrayIndex(INDEX_NONE);
	EquipSlot->EquipItem(EquippedGridItem);
	
	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
	
	ForceLayoutPrepass();
	
	// EquippedGridItem.GetItemWidget()->ForceLayoutPrepass();
	UE_LOG(LogInventory, Warning, TEXT("0. Widget Positon After Equip Function: %s"), *UBxWidgetUtils::GetCachedWidgetPosition(EquippedGridItem.GetItemWidget()).ToString());
}

bool UInv_SpatialInventory::CanEquipGrabbedItem(const UInv_EquippedGridSlot* EquippedSlot) const
{
	return !EquippedSlot->IsOccupied() 
			&& GrabbedGridItem 
			&& GrabbedGridItem->GetItemTag().MatchesTag(EquippedSlot->GetEquipmentTag());
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


