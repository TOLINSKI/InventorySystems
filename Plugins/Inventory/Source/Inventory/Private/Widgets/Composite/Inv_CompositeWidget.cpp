// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Composite/Inv_CompositeWidget.h"

void UInv_CompositeWidget::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_CompositeWidget::Expand()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}