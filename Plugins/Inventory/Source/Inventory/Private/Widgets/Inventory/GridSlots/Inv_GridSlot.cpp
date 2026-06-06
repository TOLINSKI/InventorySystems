// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"

#include "Components/Image.h"

void UInv_GridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	Image_GridSlot->SetDesiredSizeOverride(SlotSize);
}
