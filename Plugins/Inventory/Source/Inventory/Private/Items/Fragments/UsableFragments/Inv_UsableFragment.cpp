// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Fragments/UsableFragments/Inv_UsableFragment.h"

#include "Inventory.h"

void FInv_UsableFragment::OnUsed(APlayerController* PlayerController) const
{
	for (const auto& Modifier : ValueModifiers)
	{
		const FInv_UsableModifier& UsableModifier = Modifier.Get();
		UsableModifier.OnUsed(PlayerController);
	}
}

void FInv_UsableFragment::InitFragment()
{
	FInv_ItemFragment::InitFragment();
	for (auto& Modifier : ValueModifiers)
	{
		FInv_LabeledValueFragment& LabeledValueFragment = Modifier.GetMutable();
		LabeledValueFragment.InitFragment();
	}
}

void FInv_UsableFragment::ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const
{
	for (auto& Modifier : ValueModifiers)
	{
		const FInv_LabeledValueFragment& LabeledValueFragment = Modifier.Get();
		LabeledValueFragment.ApplyToCompositeWidget(Widget);
	}
}

void FInv_UsableHealthPotion::OnUsed(APlayerController* PlayerController) const
{
	const FString Msg = FString::Printf(TEXT("Used Health Potion: +%dHP"), static_cast<int32>(GetValue()));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}

void FInv_UsableManaPotion::OnUsed(APlayerController* PlayerController) const
{
	const FString Msg = FString::Printf(TEXT("Used Mana Potion: +%dMP"), static_cast<int32>(GetValue()));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Cyan, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}
