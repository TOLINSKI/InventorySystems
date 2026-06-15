// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types/Inv_InventoryTypes.h"

#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "Inv_ItemSpec.generated.h"

struct FInv_ItemFragment;
class UInv_InventoryItem;

USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemSpec
{
	GENERATED_BODY()

public:
	UInv_InventoryItem* CreateItem(UObject* Outer) const;

	void SpawnItem(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation, int32 StackCount = 1) const;
	
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	
	FGameplayTag GetItemTag() const { return ItemTag; }

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragment() const; 
	
	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	T* GetMutableFragment(); 
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<AActor> ItemActorClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	EInv_ItemCategory ItemCategory { EInv_ItemCategory::None };
	
	UPROPERTY(EditAnywhere, Category="Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag ItemTag;
};

template<typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemSpec::GetFragment() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	
	return nullptr;
};

template<typename T> requires std::derived_from<T, FInv_ItemFragment>
T* FInv_ItemSpec::GetMutableFragment()
{
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	
	return nullptr;
};