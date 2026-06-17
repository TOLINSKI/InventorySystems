// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeWidgetLeaf.h"
#include "Inv_CompLeafWidget_Header.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_CompLeafWidget_Header : public UInv_CompositeWidgetLeaf
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", DisplayName="Construct Header")
	void BP_ConstructHeader(UTexture2D* Icon, const FText& Title);
};
