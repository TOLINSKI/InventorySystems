// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeWidget.h"
#include "Inv_CompositeWidgetContainer.generated.h"

UCLASS()
class INVENTORY_API UInv_CompositeWidgetContainer : public UInv_CompositeWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
public:
	virtual void ApplyFunction(FuncType Function) override;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UInv_CompositeWidget>> CompositeWidgets;
};
