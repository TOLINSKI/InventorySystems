// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"

#include "Types/Inv_GridTypes.h"
#include "Types/Inv_InventoryTypes.h"

#include "Inv_SpatialInventory.generated.h"

class UWidgetSwitcher;
class UInv_InventoryGrid;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryWidgetBase
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void SetGridCategory(EInv_ItemCategory Category);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnCategorySelected(EInv_ItemCategory Category);

	virtual FInv_SlotAvailabilityResult GetGridAvailability(UInv_ItemComponent* ItemComponent) const override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Grid_Switcher;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftables;
};
