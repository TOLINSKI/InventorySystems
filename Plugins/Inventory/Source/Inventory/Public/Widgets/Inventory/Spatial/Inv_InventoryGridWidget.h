// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Types/Inv_GridTypes.h"

#include "Inv_InventoryGridWidget.generated.h"

class UInv_GridSlot;
class UGridPanel;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	
	virtual void NativePreConstruct() override;
	
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
	void PopulateGrid();
	
	UPROPERTY(BlueprintReadWrite, Category="Inventory", meta = (BindWidget))
	TObjectPtr<UGridPanel> GridPanel_Items; 

private:
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;
};
