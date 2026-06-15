// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Manifest//Inv_itemSpec.h"

#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_StackFragment.h"

UInv_InventoryItem* FInv_ItemSpec::CreateItem(UObject* Outer) const
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(Outer);
	Item->SetItemSpec(*this);
	return Item;
}

void FInv_ItemSpec::SpawnItem(const UObject* WorldContextObject, const FVector& Location,
	const FRotator& Rotation, int32 StackCount) const
{
	AActor* SpawnedItem = WorldContextObject->GetWorld()->SpawnActor(ItemActorClass, &Location, &Rotation);
	check(IsValid(SpawnedItem));
	
	UInv_ItemComponent* ItemComponent = SpawnedItem->GetComponentByClass<UInv_ItemComponent>();
	check(IsValid(ItemComponent));
	
	if (FInv_StackFragment* StackFrag = ItemComponent->GetMutableItemSpec().GetMutableFragment<FInv_StackFragment>())
	{
		StackFrag->SetStackCount(StackCount);
	}
}
