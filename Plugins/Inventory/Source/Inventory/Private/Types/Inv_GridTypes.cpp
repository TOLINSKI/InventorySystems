// Copyright Chaos Box Games 2026, All rights reserved.

#include "Types/Inv_GridTypes.h"

#include "Inventory.h"
#include "Items/Inv_InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Items/Fragments/Inv_StackFragment.h"
#include "Widgets/BxWidgetUtils.h"
#include "Widgets/Inv_WidgetUtils.h"
#include "Widgets/Item/Inv_ItemPopUp.h"
#include "Widgets/Item/Inv_ItemWidget.h"

FInv_GridItem::FInv_GridItem(UInv_InventoryItem* InItem, UInv_ItemWidget* InItemWidget, int32 InArrayIndex,
                             const FIntPoint& InGridSpan, int32 InStackCount, int32 InMaxStackCount)
: Item(InItem)
, ItemWidget(InItemWidget)
, ArrayIndex(InArrayIndex)
, GridSpan(InGridSpan) 
, StackCount(InStackCount)
, MaxStackCount(InMaxStackCount)
{}

FInv_GridItem::~FInv_GridItem()
{
	if (IsValid(ItemWidget))
	{
		ItemWidget->RemoveFromParent();
		ItemWidget = nullptr;
	}
}

bool FInv_GridItem::operator==(const FInv_GridItem& Other) const
{
	return Other.GetIndex() == GetIndex();
}

void FInv_GridItem::UpdateStackCountUI() const
{
	GetItemWidget()->StackCountChanged(StackCount);
}

bool FInv_GridItem::IsIndexOccupied(const int32 Index) const
{
	return ArrayIndex == Index;
}

UInv_InventoryItem* FInv_GridItem::GetItem() const
{
	return Item.Get();
}

UInv_ItemWidget* FInv_GridItem::GetItemWidget() const
{
	return ItemWidget.Get();
}

bool FInv_GridItem::IsStackable() const
{
	return GetItem()->GetItemSpec().GetFragment<FInv_StackFragment>() != nullptr;
}

bool FInv_GridItem::IsConsumable() const
{
	return GetItem()->GetItemSpec().GetItemCategory() == EInv_ItemCategory::Consumable;
}

FGameplayTag FInv_GridItem::GetItemTag() const
{
	return GetItem()->GetItemTag();
}

void FInv_GridGrabQuery::UpdateGrabbedItemPosition(const FVector2D& MousePosition) const
{
	const FVector2D NewPosition = GetRelativeWidgetPosition(MousePosition);
	
	UInv_ItemWidget* Widget = GetGridItem()->GetItemWidget();
	Widget->SetPositionInViewport(NewPosition, false);
	
	// UE_LOG(LogInventory, Display, TEXT("Init Grab Position: %s"), *InitMousePosition.ToString());
	// UE_LOG(LogInventory, Display, TEXT("Init Widget Position: %s"), *InitWidgetPosition.ToString());
	// UE_LOG(LogInventory, Display, TEXT("-------------------------------------------------------------"));
	// UE_LOG(LogInventory, Display, TEXT("Current Grab Position: %s"), *MousePosition.ToString());
	// UE_LOG(LogInventory, Display, TEXT("Current Widget Position: %s"), *NewWidgetPosition.ToString());
	// UE_LOG(LogInventory, Display, TEXT("-------------------------------------------------------------"));
	// UE_LOG(LogInventory, Display, TEXT("Position Relative to Mouse: %s"), *NewPosition.ToString());
	// UE_LOG(LogInventory, Display, TEXT("Position From GetWidgetPosition(): %s"), *UBxWidgetUtils::GetWidgetPosition(Widget).ToString());
	// UE_LOG(LogInventory, Display, TEXT("Bottom Right From GetWidgetBottomRight(): %s"), *UBxWidgetUtils::GetWidgetBottomRight(Widget).ToString());
	// UE_LOG(LogInventory, Display, TEXT("==========================================================="));
}

void FInv_GridGrabQuery::StartGrabbing(FInv_GridItem& InGridItem, const FVector2D& MouseCursorPosition)
{
	ResetQuery();
	bIsGrabbing = true;
	
	GrabbedItem = &InGridItem;
	LastPossibleIndex = GrabbedItem->GetIndex();

	UInv_ItemWidget* Widget = InGridItem.GetItemWidget();
	InitWidgetPosition = UBxWidgetUtils::GetWidgetPosition(Widget);
	InitMousePosition = MouseCursorPosition;
	DistMouseToWidget = InitWidgetPosition - InitMousePosition;
}

void FInv_GridGrabQuery::ResetQuery()
{
	LastPossibleIndex = INDEX_NONE;
	
	GrabbedItem = nullptr;
	
	StackableGridItem = nullptr;
	
	BlockingGridItems.Empty();
	
	bIsGrabbing = false;
}

void FInv_GridGrabQuery::StopGrabbing()
{
	bIsGrabbing = false;
}

UUserWidget* FInv_GridGrabQuery::GetWidget() const
{
	return GetGridItem()->GetItemWidget();
}

void FInv_GridGrabQuery::ResetIndex()
{
	SetPossibleIndex(GetGridItem()->GetIndex());
}

FVector2D FInv_GridGrabQuery::GetRelativeWidgetPosition(const FVector2D& MousePosition) const
{
	return MousePosition + DistMouseToWidget;
}

void FInv_GridPopUp::Init(FInv_GridItem& GridItem, UUserWidget* PopUpMenu)
{
	if (IsValid(PopUpMenu))
	{
		PopUpWidget = PopUpMenu;
	}
	else
	{
		check(IsValid(PopUpWidget));
	}
	
	TargetGridItem = &GridItem;
}

