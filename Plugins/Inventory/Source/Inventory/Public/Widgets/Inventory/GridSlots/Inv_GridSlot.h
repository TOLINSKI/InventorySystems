// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_GridSlot.generated.h"

class UWidgetSwitcher;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInv_GridSlotAvailabilityChanged, EInv_GridSlotState, Availability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInv_GridSlotPressEvent, UInv_GridSlot*, GridSlot, const FPointerEvent&, InMouseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInv_GridSlotEvent, UInv_GridSlot*, GridSlot);

UCLASS(BlueprintType, Blueprintable)
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	
public:
	void SetArrayIndex(int32 Index) { ArrayIndex = Index; }
	int32 GetArrayIndex() const { return ArrayIndex; }
	
	bool IsOccupied() const;
	
	EInv_GridSlotState GetGridSlotAvailability();
	
	void SetGridSlotAvailability(EInv_GridSlotState Availability);
	
	void SetSlotSize(FIntPoint Size);
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FInv_GridSlotPressEvent OnGridSlotPressed;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FInv_GridSlotPressEvent OnGridSlotUnPressed;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FInv_GridSlotEvent OnGridSlotBeginHover;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FInv_GridSlotEvent OnGridSlotEndHover;
	
	void ResetGridItem() { GridItem = nullptr; }
	void SetGridItem(FInv_GridItem* InGridItem) { GridItem = InGridItem; }
	FInv_GridItem* GetGridItem() const { return GridItem; }
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	int32 ArrayIndex;
	
	virtual void OnAvailabilityChanged(EInv_GridSlotState Availability);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void BP_OnAvailabilityChanged(EInv_GridSlotState Availability);
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FInv_GridSlotAvailabilityChanged OnGridSlotAvailabilityChanged;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush Brush_Unoccupied;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush Brush_Occupied;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush Brush_Selected;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush Brush_Disabled;
	
	FInv_GridItem* GridItem;
};
