// Copyright Chaos Box Games 2026, All rights reserved.

#include "Types/Inv_GridTypes.h"

#include "Inventory.h"
#include "Items/Inv_InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Widgets/Inv_WidgetUtils.h"
#include "Widgets/Item/Inv_ItemWidget.h"

FInv_GridItem::FInv_GridItem(UInv_InventoryItem* InItem, UInv_ItemWidget* InItemWidget, int32 InArrayIndex,
                             const FIntPoint& InGridSpan, int32 InStackCount)
: Item(InItem)
, ItemWidget(InItemWidget)
, ArrayIndex(InArrayIndex)
, GridSpan(InGridSpan) 
, StackCount(InStackCount)
{}

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

bool FInv_GrabbedQueryResult::HasFoundPossibleIndex() const
{
	return LastPossibleIndex != INDEX_NONE;
}

void FInv_GrabbedQueryResult::ResetResult()
{
	LastPossibleIndex = INDEX_NONE;
	
	GrabbedItem = {};
	
	StackableGridItem = {};
	
	BlockingGridItems = {};
}

bool FInv_GrabbedQuery::IsGrabbing() const
{
	return GetWidget() != nullptr;
}

void FInv_GrabbedQuery::UpdateGrabbedItemPosition(const FVector2D& MousePosition) const
{
	UInv_ItemWidget* Widget = GetGridItem().GetItemWidget();
	const FVector2D Difference = (MousePosition - InitGrabPosition); 
	const FVector2D NewWidgetPosition = InitWidgetPosition + Difference;
	Widget->SetPositionInViewport(NewWidgetPosition, false);
	
	const FVector2D TestWidgetPosition = UInv_WidgetUtils::GetWidgetPosition(Widget);
	UE_LOG(LogInventory, Display, TEXT("Init Grab Position: %s"), *InitGrabPosition.ToString());
	UE_LOG(LogInventory, Display, TEXT("Init Widget Position: %s"), *InitWidgetPosition.ToString());
	UE_LOG(LogInventory, Display, TEXT("-------------------------------------------------------------"));
	UE_LOG(LogInventory, Display, TEXT("Current Grab Position: %s"), *MousePosition.ToString());
	UE_LOG(LogInventory, Display, TEXT("Current Widget Position: %s"), *NewWidgetPosition.ToString());
	UE_LOG(LogInventory, Display, TEXT("Current Difference: %s"), *Difference.ToString());
	UE_LOG(LogInventory, Display, TEXT("-------------------------------------------------------------"));
	UE_LOG(LogInventory, Display, TEXT("Test Widget Position: %s"), *TestWidgetPosition.ToString());
	UE_LOG(LogInventory, Display, TEXT("==========================================================="));
}

void FInv_GrabbedQuery::StartGrabbing(const FInv_GridItem& InGridItem, const FVector2D& MouseCursorPosition)
{
	Result.ResetResult();
	Result.GrabbedItem = InGridItem;
	InitGrabPosition = MouseCursorPosition;
	
	UInv_ItemWidget* Widget = InGridItem.GetItemWidget();
	InitWidgetPosition = UInv_WidgetUtils::GetWidgetPosition(Widget);
	
	Widget->RemoveFromParent();
	Widget->AddToViewport();
	UpdateGrabbedItemPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(Widget));
}

void FInv_GrabbedQuery::ResetQuery()
{
	Result.ResetResult();
}

FInv_GrabbedQueryResult FInv_GrabbedQuery::StopGrabbing()
{
	FInv_GrabbedQueryResult QueryCopy = Result;
	
	InitGrabPosition = {};
	InitWidgetPosition = {};
	ResetQuery();
	
	return QueryCopy;
}

UUserWidget* FInv_GrabbedQuery::GetWidget() const
{
	return GetGridItem().GetItemWidget();
}

