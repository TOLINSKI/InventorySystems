// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeWidgetLeaf.h"
#include "Inv_CompLeafWidget_Description.generated.h"

UCLASS()
class INVENTORY_API UInv_CompLeafWidget_Description : public UInv_CompositeWidgetLeaf
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", DisplayName="Construct Description")
	void BP_ConstructDescription(const FText& Description);
};
