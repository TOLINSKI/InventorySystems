// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Fragments/Inv_IconFragment.h"

#include "Widgets/Composite/Inv_CompLeafWidget_Header.h"


void FInv_IconFragment::ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const
{
	FInv_ItemFragment::ApplyToCompositeWidget(Widget);
	if (!MatchesCompositeWidget(Widget)) return;
	
	UInv_CompLeafWidget_Header* HeaderWidget = Cast<UInv_CompLeafWidget_Header>(Widget);
	HeaderWidget->BP_ConstructHeader(Texture, Title);
}

