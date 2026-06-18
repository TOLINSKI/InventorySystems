// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/Inv_InventoryTypes.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_InventoryItem;
class UInv_InventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UInv_InventoryComponent* GetInventoryComponent(APlayerController* PlayerController);
	static UInv_InventoryItem* GetGrabbedItem(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable, Category="Inventory", meta = (ExpandEnumAsExecs))
	static UInv_InventoryComponent* BP_GetInventoryComponent(EInventoryFoundPins& OutputPins, APlayerController* PlayerController);
};
