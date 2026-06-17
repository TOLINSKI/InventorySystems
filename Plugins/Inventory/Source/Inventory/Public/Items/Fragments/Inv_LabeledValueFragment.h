// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_ItemFragment.h"
#include "Inv_LabeledValueFragment.generated.h"

USTRUCT(BlueprintType, DisplayName = "Labeled Value  Fragment", meta = (Hidden))
struct INVENTORY_API FInv_LabeledValueFragment : public FInv_ItemFragment 
{
	GENERATED_BODY()
public:
	FInv_LabeledValueFragment()
	{
		SetFragmentTag(Inventory::Fragments::LabeledValue);
	}
	
	virtual void ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const override;
 
	virtual void InitFragment() override;
	
	float GetValue() const { return Value;};
	
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Label { FText::FromString("Label")};

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (EditCondition = "!bRandomizeValue"))
	float Value{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bRandomizeValue{ false };
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Min{0};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Max{0};
	
	UPROPERTY(EditAnywhere, Category = "Inventory|Label")
	bool bCollapseLabel{false};

	UPROPERTY(EditAnywhere, Category = "Inventory|Label")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category = "Inventory|Label")
	int32 MinFractionalDigits{1};
	
	UPROPERTY(EditAnywhere, Category = "Inventory|Label")
	int32 MaxFractionalDigits{1};
};

