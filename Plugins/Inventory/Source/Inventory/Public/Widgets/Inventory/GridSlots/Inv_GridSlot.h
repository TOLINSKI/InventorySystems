// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_GridSlot.generated.h"

class UWidgetSwitcher;
class UImage;

/**
 * 
 */
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
