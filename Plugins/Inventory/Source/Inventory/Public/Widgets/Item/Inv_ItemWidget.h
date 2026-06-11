// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS(Abstract)
class INVENTORY_API UInv_ItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UImage* GetIconImage() const { return Image_Icon; }
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void StackCountChanged(int32 StackCount);
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
};
