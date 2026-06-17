// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Fragments/Inv_LabeledValueFragment.h"

#include "Widgets/Composite/Inv_CompLeaf_LabeledValue.h"
#include "Widgets/Composite/Inv_CompositeWidget.h"

void FInv_LabeledValueFragment::ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const
{
	FInv_ItemFragment::ApplyToCompositeWidget(Widget);
	if (!MatchesCompositeWidget(Widget)) return;
	
	UInv_CompLeaf_LabeledValue* LabeledValueWidget = Cast<UInv_CompLeaf_LabeledValue>(Widget);
	LabeledValueWidget->BP_ConstructLabeledValue(Value, Label);
}

void FInv_LabeledValueFragment::InitFragment()
{
	FInv_ItemFragment::InitFragment();
		
	if (bRandomizeValue)
	{
		Value = FMath::FRandRange(Min, Max);
	}
	bRandomizeValue = false;
}
