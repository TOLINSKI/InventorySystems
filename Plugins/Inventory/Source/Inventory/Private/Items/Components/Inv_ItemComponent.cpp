// Copyright Chaos Box Games 2026, All rights reserved.


#include "Items/Components/Inv_ItemComponent.h"

#include "Inventory.h"
#include "Items/Fragments/Inv_StackFragment.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "InvItemComponent"

UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	PickupMessage = LOCTEXT("PickupMessage", "Pick Up");
}

void UInv_ItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemSpec);
}

void UInv_ItemComponent::PickUp()
{
	BP_PickedUp();
	GetOwner()->Destroy();
}

void UInv_ItemComponent::SetItemSpec(FInv_ItemSpec ItemSpecCopy)
{
	ItemSpec = MoveTemp(ItemSpecCopy);
}

#undef LOCTEXT_NAMESPACE


