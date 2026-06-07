// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inv_InventoryTypes.h"

#include "GameplayTagContainer.h"

#include "Inv_ItemSpec.generated.h"

class UInv_InventoryItem;

USTRUCT(BlueprintType)
struct FInv_ItemSpec
{
	GENERATED_BODY()

public:
	UInv_InventoryItem* CreateItem(UObject* Outer) const;
	
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	
	FGameplayTag GetItemTag() const { return ItemTag; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	EInv_ItemCategory ItemCategory { EInv_ItemCategory::None };
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag ItemTag;
};

