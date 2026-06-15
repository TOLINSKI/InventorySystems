// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Item/Inv_ItemPopUp.h"

void UInv_ItemPopUp::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}
