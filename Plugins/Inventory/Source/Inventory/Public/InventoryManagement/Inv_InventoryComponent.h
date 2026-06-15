// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_ItemComponent;
class UInv_InventoryItem;
class UInv_InventoryWidgetBase;
struct FInv_SlotAvailabilityResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryStackChanged, const FInv_SlotAvailabilityResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryActionFailed);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract, Blueprintable, BlueprintType)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintAssignable, Category="Inv Events")
	FInventoryItemChanged OnItemAdded;
	
	UPROPERTY(BlueprintAssignable, Category="Inv Events")
	FInventoryItemChanged OnItemRemoved;

	UPROPERTY(BlueprintAssignable, Category="Inv Events")
	FInventoryActionFailed OnItemAddingFailed;
	
	UPROPERTY(BlueprintAssignable, Category="Inv Events")
	FInventoryStackChanged OnStackChanged;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);
	
	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);
	
	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UInv_ItemComponent* ItemComponents,  int32 StackCount, int32 Remainder);
	
	UFUNCTION(Server, Reliable)
	void Server_DropItem(UInv_InventoryItem* Item, int32 AmountToRemove, bool bSpawnDroppedItem = true);
	
	void SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount) const;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void OpenInventoryMenu();
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void CloseInventoryMenu();
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void ToggleInventoryMenu();
	
	void AddRepSubObj(UObject* SubObj);
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsInventoryOpen() const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsValidInventory() const;
	
private:
	TWeakObjectPtr<APlayerController> OwningPlayer;
	
	void ConstructInventory();
	
	UPROPERTY(Replicated)
	FInv_InventoryFastArray InventoryArray;

	UPROPERTY()
	TObjectPtr<UInv_InventoryWidgetBase> InventoryMenu;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_InventoryWidgetBase> InventoryMenuClass;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	bool bHideMouseCursorOnClosed;
};
