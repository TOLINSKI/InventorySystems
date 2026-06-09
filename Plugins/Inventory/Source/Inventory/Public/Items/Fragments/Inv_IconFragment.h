// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_ItemFragment.h"
#include "Inv_IconFragment.generated.h"

class UTexture2D;

USTRUCT(BlueprintType, DisplayName = "Icon Fragment")
struct INVENTORY_API FInv_IconFragment : public FInv_ItemFragment 
{
	GENERATED_BODY()
	
	FInv_IconFragment();
	
	UTexture2D* GetTexture() const { return Texture; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Texture;
};

inline FInv_IconFragment::FInv_IconFragment()
{
	SetFragmentTag(Inventory::Fragments::Icon);
}


