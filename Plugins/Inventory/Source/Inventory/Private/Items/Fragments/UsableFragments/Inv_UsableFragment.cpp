// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Fragments/UsableFragments/Inv_UsableFragment.h"

#include "Inventory.h"

void FInv_UsableHealthPotion::OnUsed(APlayerController* PlayerController)
{
	const FString Msg = FString::Printf(TEXT("Used Health Potion: +%dHP"), static_cast<int32>(HealAmount));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}

void FInv_UsableManaPotion::OnUsed(APlayerController* PlayerController)
{
	const FString Msg = FString::Printf(TEXT("Used Mana Potion: +%dMP"), static_cast<int32>(ManaAmount));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Cyan, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}
