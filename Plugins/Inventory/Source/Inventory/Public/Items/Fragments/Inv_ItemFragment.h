// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Inv_FragmentTags.h"

#include "Inv_ItemFragment.generated.h"

class UInv_InventoryItem;

USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemFragment
{
	GENERATED_BODY()

public:
	FInv_ItemFragment() {};
	
	FInv_ItemFragment(const FInv_ItemFragment&) = default; // Copy Constructor:
	FInv_ItemFragment& operator=(const FInv_ItemFragment&) = default; // Assignment Operator: 
	
	FInv_ItemFragment(FInv_ItemFragment&&) = default; // Move Constructor:
	FInv_ItemFragment& operator=(FInv_ItemFragment&&) = default; // Move Assignment Operator:  
	
	virtual ~FInv_ItemFragment() {}; // Virtual Destructor
	
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta = (Categories = "Inventory.Fragments"))
	FGameplayTag FragmentTag;
};
