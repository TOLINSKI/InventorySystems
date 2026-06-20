// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_GridSlot.h"
#include "Inv_EquippedGridSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotEvent, UInv_EquippedGridSlot*, GridSlot, const FGameplayTag&, EquipmentTag);

UCLASS()
class INVENTORY_API UInv_EquippedGridSlot : public UInv_GridSlot
{
	GENERATED_BODY()

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void SetItemWidget(UUserWidget* Widget);
	
public:
	FEquippedGridSlotEvent OnEquipSlotClicked;
	FEquippedGridSlotEvent OnEquipSlotUnClicked;
	
	void EquipGridItem(FInv_GridItem& InGridItem);
	FInv_GridItem* UnequipItem();
	
	const FGameplayTag& GetEquipmentTag() const { return EquipmentTag; }
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "Inventory.Equipment"))
	FGameplayTag EquipmentTag;
};