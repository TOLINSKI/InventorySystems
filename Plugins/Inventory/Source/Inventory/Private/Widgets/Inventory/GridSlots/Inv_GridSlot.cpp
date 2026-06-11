// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"

#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"

void UInv_GridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// Image_GridSlot->SetDesiredSizeOverride(SlotSize);
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void UInv_GridSlot::SetOccupied(bool bIsOccupied)
{
	if (bIsOccupied)
	{
		Image_GridSlot->SetBrush(Brush_Occupied);
	}
	else
	{
		Image_GridSlot->SetBrush(Brush_Unoccupied);
	}
}

bool UInv_GridSlot::IsOccupied() const
{
	return Image_GridSlot->GetBrush() == Brush_Occupied;
}

void UInv_GridSlot::SetSlotSize(FIntPoint Size)
{
	Brush_Unoccupied.SetImageSize(Size);
	Brush_Occupied.SetImageSize(Size);
	Brush_Selected.SetImageSize(Size);
	Brush_Disabled.SetImageSize(Size);
}
