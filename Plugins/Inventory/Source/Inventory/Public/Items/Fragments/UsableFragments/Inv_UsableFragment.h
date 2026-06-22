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
	
	virtual void OnUsed(APlayerController* PlayerController) {};
	virtual void OnUnUsed(APlayerController* PlayerController) {};
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
	
	virtual void OnUsed(APlayerController* PlayerController);
	virtual void OnUnUsed(APlayerController* PlayerController);
	
	bool bIsUsed { false };
	
	virtual void InitFragment() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_UsableModifier>> ValueModifiers;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bIsSingleUse { true };
};

USTRUCT(BlueprintType, DisplayName="Health Modifier")
struct INVENTORY_API FInv_HealthModifier : public FInv_UsableModifier
{
	GENERATED_BODY()
	
public:
	virtual void OnUsed(APlayerController* PlayerController) override;
};

USTRUCT(BlueprintType, DisplayName="Mana Modifier")
struct INVENTORY_API FInv_ManaModifier : public FInv_UsableModifier
{
	GENERATED_BODY()
	
public:
	virtual void OnUsed(APlayerController* PlayerController) override;
}; 

USTRUCT(BlueprintType, DisplayName="Strength Modifier")
struct INVENTORY_API FInv_StrengthModifier : public FInv_UsableModifier
{
	GENERATED_BODY()
	
public:
	virtual void OnUsed(APlayerController* PlayerController) override;
	virtual void OnUnUsed(APlayerController* PlayerController) override;
};

USTRUCT(BlueprintType, DisplayName= "Equipment Fragment")
struct INVENTORY_API FInv_EquipmentFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
	virtual void InitFragment() override;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> EquipmentClass;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FName EquipmentSocket;
};