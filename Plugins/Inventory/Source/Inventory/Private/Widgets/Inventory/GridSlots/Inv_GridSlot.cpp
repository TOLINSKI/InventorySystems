// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"

#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Inv_InventoryStatics.h"

void UInv_GridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void UInv_GridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnGridSlotBeginHover.Broadcast(this);
}

void UInv_GridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	OnGridSlotEndHover.Broadcast(this);
}

FReply UInv_GridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnGridSlotPressed.Broadcast(this, InMouseEvent);
	return FReply::Handled();
}

FReply UInv_GridSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnGridSlotUnPressed.Broadcast(this, InMouseEvent);
	return FReply::Handled();
}

bool UInv_GridSlot::IsOccupied() const
{
	const FSlateBrush& Brush = Image_GridSlot->GetBrush(); 
	return Brush == Brush_Occupied || Brush == Brush_Disabled;
}

EInv_GridSlotState UInv_GridSlot::GetGridSlotAvailability()
{
	if (Image_GridSlot->GetBrush() == Brush_Unoccupied) return EInv_GridSlotState::Unoccupied;
	if (Image_GridSlot->GetBrush() == Brush_Occupied) return EInv_GridSlotState::Occupied;
	if (Image_GridSlot->GetBrush() == Brush_Selected) return EInv_GridSlotState::Selected;
	if (Image_GridSlot->GetBrush() == Brush_Disabled) return EInv_GridSlotState::Disabled;
	return EInv_GridSlotState::Disabled;
}

void UInv_GridSlot::SetGridSlotAvailability(EInv_GridSlotState Availability)
{
	switch (Availability)
	{
	case EInv_GridSlotState::Occupied: Image_GridSlot->SetBrush(Brush_Occupied); break;
	case EInv_GridSlotState::Unoccupied: Image_GridSlot->SetBrush(Brush_Unoccupied); break;
	case EInv_GridSlotState::Disabled: Image_GridSlot->SetBrush(Brush_Disabled); break;
	case EInv_GridSlotState::Selected: 
		Image_GridSlot->SetBrush(Brush_Selected); break;
	default: break;
	}
	
	OnAvailabilityChanged(Availability);
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
