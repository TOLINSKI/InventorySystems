// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"
#include "Types/Inv_GridTypes.h"
#include "Types/Inv_InventoryTypes.h"

#include "Inv_SpatialInventory.generated.h"

class UInv_EquippedGridSlot;
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
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void PlaceGrabbedItemBackOnGrid();
	void UnequipGrabbedItem();

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	// Begin UInv_InventoryWidgetBase Interface
	virtual FInv_SlotAvailabilityResult GetGridAvailability(UInv_ItemComponent* ItemComponent) const override;
	// End UInv_InventoryWidgetBase Interface
	
protected:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SwitchGridCategory(EInv_ItemCategory Category);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void OnCategorySelected(EInv_ItemCategory Category);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	bool ShouldDropItem();

	virtual UInv_InventoryItem* GetGrabbedItem() const override;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	UUserWidget* GetGrabbedWidget() const;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	UInv_InventoryGrid* GetActiveGrid() const;
	
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
	
	void OnGridBeginGrabItem(FInv_GridItem& GridItem);
	void HandleEndGrabbingPreviouslyEquippedItem(bool bItemPlacedOnGrid);
	void HandlePreviouslyEquippedItem(bool bItemPlacedOnGrid);

	void OnGridEndGrabItem(bool bItemPlacedOnGrid);
	
	FInv_GridItem* GrabbedGridItem;
	
	bool CanEquipGrabbedItem(const UInv_EquippedGridSlot* EquippedSlot) const;
	
	UFUNCTION()
	void OnEquipSlotPressed(UInv_EquippedGridSlot* EquipSlot, const FGameplayTag& EquipmentTag);
	void EquipFreshItem(UInv_EquippedGridSlot* EquipSlot);
	void EquipPreviouslyEquippedItem(UInv_EquippedGridSlot* ExistingEquipSlot);

	UFUNCTION()
	void OnEquipSlotUnPressed(UInv_EquippedGridSlot* EquipSlot, const FGameplayTag& EquipmentTag);
	
	UFUNCTION()
	void OnEquipSlotBeginHover(UInv_GridSlot* EquipSlot);

	UFUNCTION()
	void OnEquipSlotEndHover(UInv_GridSlot* EquipSlot);
	
	UPROPERTY()
	TArray<FInv_GridItem> EquippedGridItems; 
	
	bool bHasChangedGrabbedItemGridPossibilities { false };
};
