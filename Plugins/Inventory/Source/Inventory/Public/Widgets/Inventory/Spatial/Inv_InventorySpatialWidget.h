// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"

#include "Types/Inv_GridTypes.h"

#include "Inv_InventorySpatialWidget.generated.h"

class UWidgetSwitcher;
class UInv_InventoryGridWidget;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventorySpatialWidget : public UInv_InventoryWidgetBase
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void SetGridCategory(EInv_ItemCategory Category);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnCategorySelected(EInv_ItemCategory Category);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Grid_Switcher;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGridWidget> Grid_Equippables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGridWidget> Grid_Consumables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGridWidget> Grid_Craftables;
};
