// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/Inv_InventoryStatics.h"

#include "InventoryManagement/Inv_InventoryComponent.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(APlayerController* PlayerController)
{
	return PlayerController->FindComponentByClass<UInv_InventoryComponent>();
}

UInv_InventoryComponent* UInv_InventoryStatics::BP_GetInventoryComponent(EInventoryFoundPins& OutputPins,
                                                                         APlayerController* PlayerController)
{
	UInv_InventoryComponent* InventoryComponent = GetInventoryComponent(PlayerController);
	OutputPins = IsValid(InventoryComponent) ? EInventoryFoundPins::Found : EInventoryFoundPins::NotFound;
	return InventoryComponent;
}
