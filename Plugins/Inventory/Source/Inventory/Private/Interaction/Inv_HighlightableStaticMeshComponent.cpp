// Copyright Chaos Box Games 2026, All rights reserved.


#include "Interaction/Inv_HighlightableStaticMeshComponent.h"

void UInv_HighlightableStaticMeshComponent::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UInv_HighlightableStaticMeshComponent::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
