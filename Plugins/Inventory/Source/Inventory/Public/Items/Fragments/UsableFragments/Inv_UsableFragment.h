// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Items/Fragments/Inv_ItemFragment.h"
#include "StructUtils/InstancedStruct.h"
#include "Items/Fragments/Inv_LabeledValueFragment.h"
#include "Inv_UsableFragment.generated.h"

USTRUCT(meta = (Hidden))
struct INVENTORY_API FInv_UsableModifier : public FInv_LabeledValueFragment
{
	GENERATED_BODY()
	
	virtual void OnUsed(APlayerController* PlayerController) const {};
};

USTRUCT(BlueprintType, DisplayName= "Usable Fragment")
struct INVENTORY_API FInv_UsableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
	FInv_UsableFragment()
	{
		SetFragmentTag(Inventory::Fragments::Usable);
	}
	
	virtual void ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const override;
	
	virtual void OnUsed(APlayerController* PlayerController) const;
	
	virtual void InitFragment() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_UsableModifier>> ValueModifiers;
};

USTRUCT(BlueprintType, DisplayName="Usable Health Potion")
struct INVENTORY_API FInv_UsableHealthPotion : public FInv_UsableModifier
{
	GENERATED_BODY()
	
public:
	virtual void OnUsed(APlayerController* PlayerController) const override;
};

USTRUCT(BlueprintType, DisplayName="Usable Mana Potion")
struct INVENTORY_API FInv_UsableManaPotion : public FInv_UsableModifier
{
	GENERATED_BODY()
	
public:
	virtual void OnUsed(APlayerController* PlayerController) const override;
};