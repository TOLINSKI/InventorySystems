// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inv_CompositeWidget.generated.h"

struct FInv_ItemFragment;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_CompositeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	const FGameplayTag& GetFragmentTag() const { return FragmentTag; }
	
	virtual void Collapse();
	void Expand();

	// Functor that takes UInv_CompositeWidget* param and returns void
	using FuncType = TFunction<void(UInv_CompositeWidget*)>; 
	virtual void ApplyFunction(FuncType Function) {}
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag;
};
