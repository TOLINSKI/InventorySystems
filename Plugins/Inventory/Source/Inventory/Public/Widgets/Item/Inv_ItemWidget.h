// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemWidget.generated.h"

struct FPointerEvent;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryWidgetEvent, UInv_ItemWidget*, ItemWidget, const FPointerEvent&, MouseEvent);

/**
 * 
 */
UCLASS(Abstract)
class INVENTORY_API UInv_ItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
public:
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInventoryWidgetEvent OnItemClicked;
	
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInventoryWidgetEvent OnItemUnclicked;
	
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInventoryWidgetEvent OnItemBeginHovering;
	
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInventoryWidgetEvent OnItemEndHovering;
	
	UImage* GetIconImage() const { return Image_Icon; }
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void StackCountChanged(int32 StackCount);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
	
	bool bIsMouseInside { false };
};
