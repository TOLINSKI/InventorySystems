// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/Inv_InventoryStatics.h"

#include "InventoryManagement/Inv_InventoryComponent.h"
#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(APlayerController* PlayerController)
{
	return PlayerController->FindComponentByClass<UInv_InventoryComponent>();
}

UInv_InventoryItem* UInv_InventoryStatics::GetGrabbedItem(APlayerController* PlayerController)
{
	UInv_InventoryComponent* InvComp = GetInventoryComponent(PlayerController);
	check(InvComp != nullptr);
	
	UInv_InventoryWidgetBase* InvMenu = InvComp->GetInventoryMenu();
	check(InvMenu != nullptr);
	
	return InvMenu->GetGrabbedItem();
}

UInv_InventoryComponent* UInv_InventoryStatics::BP_GetInventoryComponent(EInventoryFoundPins& OutputPins,
                                                                         APlayerController* PlayerController)
{
	UInv_InventoryComponent* InventoryComponent = GetInventoryComponent(PlayerController);
	OutputPins = IsValid(InventoryComponent) ? EInventoryFoundPins::Found : EInventoryFoundPins::NotFound;
	return InventoryComponent;
}
