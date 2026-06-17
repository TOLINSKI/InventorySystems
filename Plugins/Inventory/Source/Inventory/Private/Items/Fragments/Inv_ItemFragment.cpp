// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Fragments/Inv_ItemFragment.h"

#include "Widgets/Composite/Inv_CompositeWidget.h"

void FInv_ItemFragment::ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const
{
	if (!MatchesCompositeWidget(Widget)) return;
	
	Widget->Expand();
}

bool FInv_ItemFragment::MatchesCompositeWidget(const UInv_CompositeWidget* Widget) const
{
	return Widget->GetFragmentTag() == FragmentTag;
}
