// Copyright Chaos Box Games 2026, All rights reserved.


#include "Items/Components/Inv_ItemComponent.h"

#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "InvItemComponent"

UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	PickupMessage = LOCTEXT("PickupMessage", "Pick Up");
}

void UInv_ItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemSpec);
}

#undef LOCTEXT_NAMESPACE


