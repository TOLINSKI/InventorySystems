// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/FastArray/Inv_FastArray.h"

#include "InventoryManagement/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "items/Manifest/Inv_ItemSpec.h"


TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const
{
	TArray<UInv_InventoryItem*> Result;
	Result.Reserve(Items.Num());
	
	for (const auto& Item : Items)
	{
		if (!IsValid(Item.Get())) continue;
		Result.Add(Item.Get());
	}
	
	return Result;
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	UInv_InventoryComponent* InvComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!ensure(IsValid(InvComponent))) return;
	
	for (int32 Index : RemovedIndices)
	{
		InvComponent->OnItemRemoved.Broadcast(Items[Index].Get());
	}
}

void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	// Only Called on Clients!
	UInv_InventoryComponent* InvComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!ensure(IsValid(InvComponent))) return;
	
	for (int32 Index : AddedIndices)
	{
		InvComponent->OnItemAdded.Broadcast(Items[Index].Get());
	}
}

UInv_InventoryItem* FInv_InventoryFastArray::AddItem(UInv_ItemComponent* ItemComponent)
{
	check(OwnerComponent);
	
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	check(IsValid(InventoryComponent));
	
	FInv_InventoryFastArrayItem& Item = Items.AddDefaulted_GetRef();
	Item.SetObject(ItemComponent->GetItemSpec().CreateItem(OwningActor));
	
	InventoryComponent->AddRepSubObj(Item.Get());
	MarkItemDirty(Item);
	return Item.Get();
}

UInv_InventoryItem* FInv_InventoryFastArray::AddItem(UInv_InventoryItem* Item)
{
	UInv_InventoryComponent* InvComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!ensure(IsValid(InvComponent)) || !ensure(InvComponent->GetOwner()->HasAuthority())) return nullptr;
	
	FInv_InventoryFastArrayItem& ItemEntry = Items.AddDefaulted_GetRef();
	ItemEntry.SetObject(Item);
	
	MarkItemDirty(ItemEntry);
	return Item;
}

void FInv_InventoryFastArray::RemoveItem(UInv_InventoryItem* Item)
{
	for (auto It = Items.CreateIterator(); It; ++It)
	{
		FInv_InventoryFastArrayItem& FastItem = *It;
		if (It->Get() == Item)
		{
			It.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UInv_InventoryItem* FInv_InventoryFastArray::FindItemByTag(const FGameplayTag& Tag)
{
	for (const auto& FastItem : Items)
	{
		UInv_InventoryItem* Item = FastItem.Get();
		if (Item->GetItemTag() == Tag)
		{
			return Item;
		}
	}
	
	return nullptr;
}
