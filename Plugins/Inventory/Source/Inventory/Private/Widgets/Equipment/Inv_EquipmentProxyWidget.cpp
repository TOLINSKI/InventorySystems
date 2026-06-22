// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Equipment/Inv_EquipmentProxyWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Equipement/Inv_EquipmentProxyActor.h"
#include "Kismet/GameplayStatics.h"

void UInv_EquipmentProxyWidget::StartDragging()
{
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	bIsDragging = true;
}

void UInv_EquipmentProxyWidget::StopDragging()
{
	bIsDragging = false;
	if (AInv_EquipmentProxyActor* Proxy = ProxyActor.Get())
	{
		Proxy->GetMesh()->SetWorldRotation(InitialRotation);
	}
}

void UInv_EquipmentProxyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ProxyActor = Cast<AInv_EquipmentProxyActor>(UGameplayStatics::GetActorOfClass(this, AInv_EquipmentProxyActor::StaticClass()));
	if (ProxyActor.IsValid())
	{
		InitialRotation = ProxyActor->GetMesh()->GetComponentRotation();
	}
}

void UInv_EquipmentProxyWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	OnProxyBeginHover.ExecuteIfBound(this);
}

void UInv_EquipmentProxyWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	OnProxyEndHover.ExecuteIfBound(this);
}

FReply UInv_EquipmentProxyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	OnProxyPressed.ExecuteIfBound(this, InMouseEvent);
	return FReply::Handled();
}

FReply UInv_EquipmentProxyWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnProxyUnPressed.ExecuteIfBound(this, InMouseEvent);
	return FReply::Handled();
}

void UInv_EquipmentProxyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!bIsDragging || !ProxyActor.IsValid()) return;
	
	LastMousePosition = CurrentMousePosition;
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	const float YawDelta = CurrentMousePosition.X - LastMousePosition.X;
	ProxyActor->GetMesh()->AddLocalRotation(FRotator(0.f, -YawDelta, 0.f));
}
