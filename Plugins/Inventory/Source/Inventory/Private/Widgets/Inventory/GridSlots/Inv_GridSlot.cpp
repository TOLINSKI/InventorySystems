// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"

#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"

void UInv_GridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

bool UInv_GridSlot::IsOccupied() const
{
	const FSlateBrush& Brush = Image_GridSlot->GetBrush(); 
	return Brush == Brush_Occupied || Brush == Brush_Disabled;
}

void UInv_GridSlot::SetGridSlotState(EInv_GridSlotState State)
{
	switch (State)
	{
	case EInv_GridSlotState::Occupied: Image_GridSlot->SetBrush(Brush_Occupied); break;
	case EInv_GridSlotState::Unoccupied: Image_GridSlot->SetBrush(Brush_Unoccupied); break;
	case EInv_GridSlotState::Disabled: Image_GridSlot->SetBrush(Brush_Disabled); break;
	case EInv_GridSlotState::Selected: Image_GridSlot->SetBrush(Brush_Selected); break;
	default: break;
	}
	
	OnAvailabilityChanged(State);
}

void UInv_GridSlot::SetSlotSize(FIntPoint Size)
{
	Brush_Unoccupied.SetImageSize(Size);
	Brush_Occupied.SetImageSize(Size);
	Brush_Selected.SetImageSize(Size);
	Brush_Disabled.SetImageSize(Size);
}

void UInv_GridSlot::OnAvailabilityChanged(EInv_GridSlotState Availability)
{
	OnGridSlotAvailabilityChanged.Broadcast(Availability);
	BP_OnAvailabilityChanged(Availability);
}
