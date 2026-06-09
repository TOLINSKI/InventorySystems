// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_ItemFragment.h"
#include "Inv_GridFragment.generated.h"

USTRUCT(BlueprintType, DisplayName = "Grid Fragment")
struct INVENTORY_API FInv_GridFragment : public FInv_ItemFragment 
{
	GENERATED_BODY()
	
	FInv_GridFragment();
	
	FIntPoint GetGridSpan() const { return GridSpan; }
	
private:
	/** The size in grid cells, e.g. {1,1}, {2,3} etc. */
	UPROPERTY(EditAnywhere, Category="Inventory")
	FIntPoint GridSpan { 1, 1};
	
	/** The top left cell in the grid */
	UPROPERTY(EditAnywhere, Category="Inventory")
	FIntPoint GridPosition { 0, 0 };
};

inline FInv_GridFragment::FInv_GridFragment()
{
	SetFragmentTag(Inventory::Fragments::Grid);
}
