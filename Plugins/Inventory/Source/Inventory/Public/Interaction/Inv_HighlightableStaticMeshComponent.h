// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inv_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "Inv_HighlightableStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent), DisplayName="HighlightableStaticMesh")
class INVENTORY_API UInv_HighlightableStaticMeshComponent : public UStaticMeshComponent, public IInv_Highlightable
{
	GENERATED_BODY()
	
public:
	virtual void Highlight_Implementation() override;
	virtual void UnHighlight_Implementation() override;
	
protected:
	UPROPERTY(EditAnywhere, Category= "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
