// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class INVENTORY_API UInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category="Inv")
	void ShowPickupMessage(const FText& Message);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inv")
	void HidePickupMessage();
};
