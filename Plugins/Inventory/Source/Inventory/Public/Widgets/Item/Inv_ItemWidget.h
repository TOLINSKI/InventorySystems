// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemWidget.generated.h"

struct FPointerEvent;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInv_ItemHoverEvent, UInv_ItemWidget*, ItemWidget);

UCLASS(Abstract)
class INVENTORY_API UInv_ItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
public:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	UImage* GetIconImage() const { return Image_Icon; }
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void StackCountChanged(int32 StackCount);

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInv_ItemHoverEvent OnItemBeginHover;
	
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInv_ItemHoverEvent OnItemEndHover;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
	
	bool bIsMouseInside { false };
};
