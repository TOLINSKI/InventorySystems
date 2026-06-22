// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_EquipmentProxyWidget.generated.h"

class AInv_EquipmentProxyActor;
DECLARE_DELEGATE_OneParam(FInv_EquipmentProxyWidgetEvent, class UInv_EquipmentProxyWidget* /** ProxyWidget */);
DECLARE_DELEGATE_TwoParams(FInv_EquipmentProxyWidgetMouseEvent, UInv_EquipmentProxyWidget* /** ProxyWidget */, const FPointerEvent& MouseEvent);

UCLASS()
class INVENTORY_API UInv_EquipmentProxyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FInv_EquipmentProxyWidgetEvent OnProxyBeginHover;
	FInv_EquipmentProxyWidgetEvent OnProxyEndHover;
	FInv_EquipmentProxyWidgetMouseEvent OnProxyPressed;
	FInv_EquipmentProxyWidgetMouseEvent OnProxyUnPressed;
	
	void StartDragging();
	void StopDragging();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	TWeakObjectPtr<AInv_EquipmentProxyActor> ProxyActor;
	FVector2D LastMousePosition;
	FVector2D CurrentMousePosition;
	FRotator InitialRotation;
	bool bIsDragging { false };
};
