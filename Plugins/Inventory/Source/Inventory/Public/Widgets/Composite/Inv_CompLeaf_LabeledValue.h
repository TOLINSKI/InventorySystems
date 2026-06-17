// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeWidgetLeaf.h"
#include "Inv_CompLeaf_LabeledValue.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_CompLeaf_LabeledValue : public UInv_CompositeWidgetLeaf
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", DisplayName="Construct Labled Value")
	void BP_ConstructLabeledValue(float Value, const FText& Label);
};
