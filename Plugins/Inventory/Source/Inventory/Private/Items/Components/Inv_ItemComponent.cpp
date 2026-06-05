// Copyright Chaos Box Games 2026, All rights reserved.


#include "Items/Components/Inv_ItemComponent.h"

#define LOCTEXT_NAMESPACE "InvItemComponent"

UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	PickupMessage = LOCTEXT("PickupMessage", "Pick Up");
}

#undef LOCTEXT_NAMESPACE


