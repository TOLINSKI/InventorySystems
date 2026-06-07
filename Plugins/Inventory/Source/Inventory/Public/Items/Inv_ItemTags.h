// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace Inventory
{
	namespace Equipment
	{
		namespace Weapons
		{
			INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe)
			INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sword)
		}

		namespace Cloaks
		{
			INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RedCloak)
		}

		namespace Masks
		{
			INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SteelMask)
		}
	}

	namespace Consumables
	{
		namespace Potions
		{
			namespace Red
			{
				INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small)
				INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large)
			}

			namespace Blue
			{
				INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small)
				INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large)
			}
		}
	}

	namespace Craftables
	{
		INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireFernFruit)
		INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LuminDaisy)
		INVENTORY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ScorchPetalBlossom)
	}
}

