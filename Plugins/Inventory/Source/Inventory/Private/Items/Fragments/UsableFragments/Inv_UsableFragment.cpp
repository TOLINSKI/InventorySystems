// Copyright Chaos Box Games 2026, All rights reserved.

#include "Items/Fragments/UsableFragments/Inv_UsableFragment.h"

#include "Inventory.h"

void FInv_UsableFragment::OnUsed(APlayerController* PlayerController)
{
	if (bIsUsed && !bIsSingleUse) return;
	bIsUsed = true;
	
	for (auto& Modifier : ValueModifiers)
	{
		FInv_UsableModifier& UsableModifier = Modifier.GetMutable();
		UsableModifier.OnUsed(PlayerController);
	}
}

void FInv_UsableFragment::OnUnUsed(APlayerController* PlayerController)
{
	if (!bIsUsed && !bIsSingleUse) return;
	bIsUsed = false;
	
	for (auto& Modifier : ValueModifiers)
	{
		FInv_UsableModifier& UsableModifier = Modifier.GetMutable();
		UsableModifier.OnUnUsed(PlayerController);
	}
}

void FInv_UsableFragment::InitFragment()
{
	FInv_ItemFragment::InitFragment();
	for (auto& Modifier : ValueModifiers)
	{
		auto& LabeledValueFragment = Modifier.GetMutable();
		LabeledValueFragment.InitFragment();
	}
}

void FInv_UsableFragment::ApplyToCompositeWidget(UInv_CompositeWidget* Widget) const
{
	for (auto& Modifier : ValueModifiers)
	{
		const auto& LabeledValueFragment = Modifier.Get();
		LabeledValueFragment.ApplyToCompositeWidget(Widget);
	}
}

void FInv_HealthModifier::OnUsed(APlayerController* PlayerController)
{
	const FString Msg = FString::Printf(TEXT("Used Health Potion: + %dHP"), static_cast<int32>(GetValue()));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}

void FInv_ManaModifier::OnUsed(APlayerController* PlayerController)
{
	const FString Msg = FString::Printf(TEXT("Used Mana Potion: + %dMP"), static_cast<int32>(GetValue()));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Cyan, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}

void FInv_StrengthModifier::OnUsed(APlayerController* PlayerController)
{
	const FString Msg = FString::Printf(TEXT("Strength Modified: +%d Strength Gained"), static_cast<int32>(GetValue()));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}

void FInv_StrengthModifier::OnUnUsed(APlayerController* PlayerController)
{
	const FString Msg = FString::Printf(TEXT("Strength Modified: -%d Strength Lost"), static_cast<int32>(GetValue()));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *Msg));
	UE_LOG(LogInventory, Display, TEXT("%s"), *Msg);
}

void FInv_EquipmentFragment::InitFragment()
{
	FInv_ItemFragment::InitFragment();
	ensure(EquipmentClass != nullptr);
}

