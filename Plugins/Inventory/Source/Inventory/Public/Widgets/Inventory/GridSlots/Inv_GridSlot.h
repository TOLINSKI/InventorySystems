// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_GridSlot.generated.h"

class UWidgetSwitcher;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInv_GridSlotAvailabilityChanged, EInv_GridSlotState, Availability);

UCLASS(BlueprintType, Blueprintable)
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
public:
	void SetArrayIndex(int32 Index) { ArrayIndex = Index; }
	int32 GetArrayIndex() const { return ArrayIndex; }
	
	bool IsOccupied() const;
	
	void SetGridSlotState(EInv_GridSlotState State);
	
	void SetSlotSize(FIntPoint Size);
	
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
};
