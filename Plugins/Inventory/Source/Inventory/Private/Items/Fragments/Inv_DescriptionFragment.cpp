// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Fragments/Inv_DescriptionFragment.h"

#include "Widgets/Composite/Inv_CompLeafWidget_Description.h"

void FInv_DescriptionFragment::ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const
{
	FInv_ItemFragment::ApplyToCompositeWidget(Widget);
	if (!MatchesCompositeWidget(Widget)) return;
	
	UInv_CompLeafWidget_Description* DescriptionWidget = Cast<UInv_CompLeafWidget_Description>(Widget);
	DescriptionWidget->BP_ConstructDescription(Description);
}
