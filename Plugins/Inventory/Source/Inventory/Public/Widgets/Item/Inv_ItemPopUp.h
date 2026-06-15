// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_InventoryTypes.h"
#include "Inv_ItemPopUp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInv_PopUpMenuEvent, int32, Amount);

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_ItemPopUp : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Inventory")
	void SetMaxValue(float MaxValue);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Inventory")
	void SetValue(float Value);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Inventory")
	void SetValueEnabled(bool bEnabled);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Inventory")
	void SetItemCategory(EInv_ItemCategory ItemCategory);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Invetory")
	FInv_PopUpMenuEvent OnSplitItem;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Invetory")
	FInv_PopUpMenuEvent OnDropItem;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Invetory")
	FInv_PopUpMenuEvent OnUseItem;
};
