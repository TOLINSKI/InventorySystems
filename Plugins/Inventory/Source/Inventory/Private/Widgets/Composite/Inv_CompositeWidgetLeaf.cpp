// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Composite/Inv_CompositeWidgetLeaf.h"

void UInv_CompositeWidgetLeaf::ApplyFunction(FuncType Function)
{
	Function(this);
}
