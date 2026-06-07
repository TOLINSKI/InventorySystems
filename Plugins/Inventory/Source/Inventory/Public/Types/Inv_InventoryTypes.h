// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "Inv_InventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
	None,
	Equippable,
	Consumable,
	Craftable,
};

UENUM(BlueprintType)
enum class EInventoryFoundPins : uint8
{
	Found,
	NotFound
};
