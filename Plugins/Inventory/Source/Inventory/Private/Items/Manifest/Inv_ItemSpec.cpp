// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Manifest//Inv_itemSpec.h"

#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_StackFragment.h"

UInv_InventoryItem* FInv_ItemSpec::CreateItem(UObject* Outer)
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(Outer);
	Item->SetItemSpec(*this);
	for (auto& Fragment : Item->GetMutableItemSpec().GetMutableFragments())
	{
		Fragment.GetMutable().InitFragment();
	}
	ClearFragments();
	
	return Item;
}

void FInv_ItemSpec::ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const
{
	for (const auto& Fragment : Fragments)
	{
		const FInv_ItemFragment* ItemFragment = Fragment.GetPtr<FInv_ItemFragment>();
		ItemFragment->ApplyToCompositeWidget(Widget);
	}
}

void FInv_ItemSpec::ClearFragments()
{
	for (auto& Fragment : Fragments)
	{
		Fragment.Reset();
	}
	Fragments.Empty();
}

void FInv_ItemSpec::SpawnPickupItem(const UObject* WorldContextObject, const FVector& Location,
                              const FRotator& Rotation, int32 StackCount) const
{
	
	AActor* SpawnedItem = WorldContextObject->GetWorld()->SpawnActor(ItemActorClass, &Location, &Rotation);
	check(IsValid(SpawnedItem));
	
	UInv_ItemComponent* ItemComponent = SpawnedItem->GetComponentByClass<UInv_ItemComponent>();
	check(IsValid(ItemComponent));

	ItemComponent->SetItemSpec(*this);
}
