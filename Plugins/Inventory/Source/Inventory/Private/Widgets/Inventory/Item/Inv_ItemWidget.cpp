// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Item/Inv_ItemWidget.h"


FReply UInv_ItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnItemClicked.Broadcast(this, InMouseEvent);
	
	return FReply::Handled();
}

FReply UInv_ItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnItemUnclicked.Broadcast(this, InMouseEvent);
	}
	return FReply::Handled();
}

void UInv_ItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsMouseInside = true;
	OnItemBeginHovering.Broadcast(this, InMouseEvent);
}

void UInv_ItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsMouseInside = false;
	OnItemEndHovering.Broadcast(this, InMouseEvent);
}

