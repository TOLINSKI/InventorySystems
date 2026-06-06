// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"


class UInv_InventoryWidgetBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract, Blueprintable, BlueprintType)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void OpenInventoryMenu();
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void CloseInventoryMenu();
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void ToggleInventoryMenu();
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsInventoryOpen() const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsValidInventory() const;
	
protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<APlayerController> OwningPlayer;
	
	void ConstructInventory();

	UPROPERTY()
	TObjectPtr<UInv_InventoryWidgetBase> InventoryMenu;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_InventoryWidgetBase> InventoryMenuClass;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	bool bHideMouseCursorOnClosed;
};
