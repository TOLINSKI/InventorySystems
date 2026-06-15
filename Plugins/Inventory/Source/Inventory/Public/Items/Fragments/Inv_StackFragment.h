// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_ItemFragment.h"
#include "Inv_StackFragment.generated.h"

USTRUCT(BlueprintType, DisplayName="Stack Fragment")
struct INVENTORY_API FInv_StackFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
public:
	FInv_StackFragment();
	
	int32 GetStackCount() const { return StackCount; }
	int32 GetMaxStackCount() const { return MaxStackCount; }
	
	void SetStackCount(int32 Count) { StackCount = Count;  }
	void AddStackCount(int32 Count) { StackCount += Count; }
	void SubtractStackCount(int32 Count) { StackCount -= Count; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 StackCount { 1 };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	int32 MaxStackCount { 1 };
};

inline FInv_StackFragment::FInv_StackFragment()
{
	SetFragmentTag(Inventory::Fragments::Stack);
}


