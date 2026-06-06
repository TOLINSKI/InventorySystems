// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;
	
public:
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	int32 ArrayIndex;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D SlotSize;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
};
