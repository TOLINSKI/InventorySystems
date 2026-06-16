// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Items/Fragments/Inv_ItemFragment.h"
#include "Inv_UsableFragment.generated.h"

USTRUCT(meta = (Hidden))
struct INVENTORY_API FInv_UsableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
public:
	FInv_UsableFragment();
	
	virtual void OnUsed(APlayerController* PlayerController) {};
};

inline FInv_UsableFragment::FInv_UsableFragment()
{
	SetFragmentTag(Inventory::Fragments::Usable);
}

USTRUCT(BlueprintType, DisplayName="Usable Health Potion")
struct INVENTORY_API FInv_UsableHealthPotion : public FInv_UsableFragment
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	float HealAmount { 10.f };
	
public:
	virtual void OnUsed(APlayerController* PlayerController) override;
};

USTRUCT(BlueprintType, DisplayName="Usable Mana Potion")
struct INVENTORY_API FInv_UsableManaPotion : public FInv_UsableFragment
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	float ManaAmount { 10.f };
	
public:
	virtual void OnUsed(APlayerController* PlayerController) override;
};