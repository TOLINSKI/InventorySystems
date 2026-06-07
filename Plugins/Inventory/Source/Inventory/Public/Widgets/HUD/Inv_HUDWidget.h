// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

class UInv_InfoMessageWidget;
/**
 * 
 */
UCLASS(Abstract)
class INVENTORY_API UInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inv")
	void ShowItemMessage(const FText& Message);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inv")
	void HideItemMessage();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InfoMessageWidget> InfoMessageWidget;
};
