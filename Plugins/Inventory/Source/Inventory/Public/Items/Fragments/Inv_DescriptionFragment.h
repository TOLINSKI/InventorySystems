// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_ItemFragment.h"
#include "Inv_DescriptionFragment.generated.h"

USTRUCT(BlueprintType, DisplayName = "Description Fragment")
struct INVENTORY_API FInv_DescriptionFragment : public FInv_ItemFragment 
{
	GENERATED_BODY()

public:
	FInv_DescriptionFragment();
	
	virtual void ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const override;
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta = (MultiLine = "true"))
	FText Description { FText::FromString("Description")};
};

inline FInv_DescriptionFragment::FInv_DescriptionFragment()
{
	SetFragmentTag(Inventory::Fragments::Description);
}