// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"
#include "Types/Inv_GridTypes.h"
#include "Types/Inv_InventoryTypes.h"

#include "Inv_SpatialInventory.generated.h"

class UWidgetSwitcher;
class UInv_InventoryGrid;
class UInv_ItemComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
public:
	
	// Begin UInv_InventoryWidgetBase Interface
	virtual FInv_SlotAvailabilityResult GetGridAvailability(UInv_ItemComponent* ItemComponent) const override;
	// End UInv_InventoryWidgetBase Interface
	
	UFUNCTION(BlueprintCallable)
	void SwitchGridCategory(EInv_ItemCategory Category);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnCategorySelected(EInv_ItemCategory Category);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Grid_Switcher;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftables;
	
	UInv_InventoryGrid* GetGridByCategory(EInv_ItemCategory Category) const;
};
