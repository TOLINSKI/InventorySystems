// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Fragments/Inv_GridFragment.h"
#include "Items/Fragments/Inv_IconFragment.h"
#include "Types/Inv_GridTypes.h"

#include "Types/Inv_InventoryTypes.h"

#include "Inv_InventoryGrid.generated.h"

class UInv_ItemComponent;
class UGridSlot;
class UInv_ItemWidget;
struct FInv_ItemSpec;
class UInv_InventoryItem;
class UInv_GridSlot;
class UGridPanel;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
	FInv_SlotAvailabilityResult GetSlotAvailability(const UInv_ItemComponent* ItemComponent);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	EInv_ItemCategory ItemCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 Columns;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 Rows;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	FVector2D SlotSize {32.f, 32.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;	
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void ConstructGrid();
	
	UPROPERTY(BlueprintReadWrite, Category="Inventory", meta = (BindWidget))
	TObjectPtr<UGridPanel> GridPanel_Items; 

private:
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;
	
	UPROPERTY()
	TArray<FInv_GridItem> GridItems;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_ItemWidget> ItemWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FIntPoint GridSlotSize {64, 64};
	
	bool IsMatchingCategory(UInv_InventoryItem* Item);
	
	UInv_ItemWidget* CreateItemWidget(const FInv_GridFragment* GridFragment, const FInv_IconFragment* IconFragment);
	
	void AddGridItem(const FInv_GridItem& GridItem);
	
	UFUNCTION()
	void Stack(const FInv_SlotAvailability& SlotAvailability);
	
	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);
	
	UFUNCTION()
	void AddResult(const FInv_SlotAvailabilityResult& Result);	
	
	bool IsIndexOccupied(int32 Index) const;
	
	TSet<int32> FindOccupiedSlots(int32 Index, const FIntPoint& GridSpan);

	TSet<int32> GetOccupiedIndices(int32 Index, const FIntPoint& Range2D);
	
	UInv_InventoryItem* GetItemObjectAtIndex(int32 Index) const;
	
	FInv_GridItem* GetGridItemAtIndex(int32 Index);
	
	FInv_SlotAvailabilityResult GetSlotAvailability(const FInv_ItemSpec& ItemSpec);
	

};
