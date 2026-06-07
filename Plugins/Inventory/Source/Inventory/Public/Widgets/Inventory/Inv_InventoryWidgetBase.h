// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryWidgetBase.generated.h"

class UInv_ItemComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual FInv_SlotAvailabilityResult GetSlotAvailability(UInv_ItemComponent* ItemComponent) const { return FInv_SlotAvailabilityResult(); }
};
