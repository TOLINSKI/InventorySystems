// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Types/Inv_GridTypes.h"
#include "Types/Inv_InventoryTypes.h"

#include "Inv_InventoryGrid.generated.h"

class UInv_ItemDescription;
struct FInv_IconFragment;
struct FInv_GridFragment;
class UInv_ItemPopUp;
class UInv_ItemComponent;
class UGridSlot;
class UInv_ItemWidget;
struct FInv_ItemSpec;
class UInv_InventoryItem;
class UInv_GridSlot;
class UGridPanel;

DECLARE_DELEGATE(FInv_GridEvent);

UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	FInv_SlotAvailabilityResult GetSlotAvailability(const UInv_ItemComponent* ItemComponent) const;
	
	FInv_GridEvent OnUnClicked;
	
	void DropLastGrabbedItem();
	
protected:
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TSubclassOf<UInv_ItemPopUp> PopUpMenuClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Item Description")
	TSubclassOf<UInv_ItemDescription> PopUpDescriptionClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Item Description")
	float ShowDescriptionDelay { 1.f };
	
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
	
	void ReverseLastTickGridSlotEffects();

	void UpdateGrabbedQuery();
	
	bool IsMatchingCategory(UInv_InventoryItem* Item);
	
	UInv_ItemWidget* CreateItemWidget(const FInv_GridFragment* GridFragment, const FInv_IconFragment* IconFragment);
	
	void SetGridSlotsStateInRange(EInv_GridSlotState State, int32 Index, const FIntPoint& Range2D);
	
	void PlaceOnGrid(const FInv_GridItem& GridItem);
	
	void Stack(const FInv_SlotAvailability& SlotAvailability);
	
	void RemoveGridItem(const FInv_GridItem& GridItem);
	
	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);
	
	UFUNCTION()
	void AddResult(const FInv_SlotAvailabilityResult& Result);
	
	int32 GetItemWidgetIndex(const UUserWidget* ItemWidget);

	UFUNCTION()
	void OnItemClicked(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnItemUnclicked(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent);
	void HidePopUpDescription();

	UFUNCTION()
	void OnItemBeginHovering(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent);
	
	UFUNCTION()
	void OnItemEndHovering(UInv_ItemWidget* ItemWidget, const FPointerEvent& MouseEvent);
	
	bool CanFitRange(int32 Index, const FIntPoint& Range2D) const;
	
	TSet<int32> FindOccupiedIndices(int32 Index, const FIntPoint& Range2D) const;
	
	TSet<int32> FindIndecesToOccupy(int32 Index, const FIntPoint& Range2D) const;
	
	UInv_InventoryItem* GetItemObjectAtIndex(int32 Index) const;
	
	const FInv_GridItem* FindGridItemByIndex(int32 Index) const;
	
	FInv_GridItem* FindGridItem(int32 Index);
	
	FInv_GridItem* FindGridItem(const UUserWidget* ItemWidget);
	
	FInv_GridItem* FindGridItem(UInv_GridSlot* GridSlot);
	
	FInv_GridItem* FindOverlappingGridItem(int32 Index);
	
	FInv_SlotAvailabilityResult GetSlotAvailability(const FInv_ItemSpec& ItemSpec) const;
	
	FInv_GridGrabQuery GrabbedQuery;
	
	EInv_GridSlotQuadrant GetGridSlotQuadrant(UUserWidget* Widget) const;
	
	FIntPoint ViewportPositionToGridCoordinate(const FVector2D& Position);

	FIntPoint GetGridCoordinatesByQuadrant(const FVector2D& WidgetCenter, const FIntPoint& GridSpan, EInv_GridSlotQuadrant Quadrant);
	
	UInv_GridSlot* FindGridSlotByIndex(int32 Index);
	
	UInv_GridSlot* FindGridSlotByCoordinates(const FIntPoint& GridCoordinates);
	
	
	// Begin Grid Pop Up Menu
	UPROPERTY()
	FInv_GridPopUp PopUpMenu;
	
	void CreatePopUpMenu(FInv_GridItem& GridItem);
	
	UFUNCTION()
	void OnPopUpMenuSplitAction(int32 SplitAmount);
	
	UFUNCTION()
	void OnPopUpMenuUseAction(int32 UsedAmount);
	
	UFUNCTION()
	void OnPopUpMenuDropAction(int32 DropAmount);
	
	void DropGridItem(FInv_GridItem* GridItem, int32 Amount);

	void UseGridItem(FInv_GridItem* GridItem, int32 Amount);
	// End Grid Pop Up Menu
	
	// Begin Grid Pop Up Description
	UPROPERTY()
	FInv_GridPopUp PopUpDescription;
	
	void CreatePopUpDescription(FInv_GridItem& GridItem);
	
	FTimerHandle DescriptionTimer;
	// End Grid Pop Up Description
};
