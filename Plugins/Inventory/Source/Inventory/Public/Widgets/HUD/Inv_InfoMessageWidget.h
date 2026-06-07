// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_InfoMessageWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InfoMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	void SetText(const FText& Message); 
	
	UFUNCTION()
	void ShowMessage();
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsActive() const;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", DisplayName="Show Message")
	void BP_ShowMessage();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", DisplayName="Hide Message")
	void BP_HideMessage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	FText MessageText { FText::FromString("Info Message")};
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	float MessageLifetime { 3.f };
	
	FTimerHandle MessageTimer;
};
