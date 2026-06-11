// Copyright Chaos Box Games 2026, All rights reserved.


#include "Items/Inv_InventoryItem.h"

#include "Net/UnrealNetwork.h"

void UInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemSpec);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}

void UInv_InventoryItem::SetItemSpec(const FInv_ItemSpec& Spec)
{
	ItemSpec = FInstancedStruct::Make<FInv_ItemSpec>(Spec);
}

FGameplayTag UInv_InventoryItem::GetItemTag() const
{
	return GetItemSpec().GetItemTag();
}

