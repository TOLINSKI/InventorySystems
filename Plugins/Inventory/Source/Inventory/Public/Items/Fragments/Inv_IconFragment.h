// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_ItemFragment.h"
#include "Inv_IconFragment.generated.h"

class UTexture2D;

USTRUCT(BlueprintType, DisplayName = "Icon Fragment")
struct INVENTORY_API FInv_IconFragment : public FInv_ItemFragment 
{
	GENERATED_BODY()
	
public:
	FInv_IconFragment();
	
	virtual void ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const override; 
	
	UTexture2D* GetTexture() const { return Texture; }
	
	const FText& GetTitleText() const { return Title; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Texture;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FText Title {FText::FromString("Title Text")};
};

inline FInv_IconFragment::FInv_IconFragment()
{
	SetFragmentTag(Inventory::Fragments::Icon);
}


