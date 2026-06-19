// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Item/Inv_ItemWidget.h"


void UInv_ItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInv_ItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnItemBeginHover.Broadcast(this);
}

void UInv_ItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnItemEndHover.Broadcast(this);
}

