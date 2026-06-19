// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/BxWidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

FVector2D UBxWidgetUtils::GetCachedWidgetPosition(UWidget* Widget)
{
	const FVector2D AbsoluteDesktopCoordinate = Widget->GetCachedGeometry().GetAbsolutePosition();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsoluteDesktopCoordinate, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UBxWidgetUtils::GetCachedWidgetCenter(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	const FVector2D AbsoluteSize = Widget->GetCachedGeometry().GetAbsoluteSize();
	const FVector2D AbsoluteCenter = AbsolutePosition + AbsoluteSize / 2.f;
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsoluteCenter, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UBxWidgetUtils::GetCachedWidgetBottomRight(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	const FVector2D AbsoluteSize = Widget->GetCachedGeometry().GetAbsoluteSize();
	const FVector2D AbsoluteBottomRight = AbsolutePosition + AbsoluteSize;
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsoluteBottomRight, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

bool UBxWidgetUtils::IsPositionBoundByCachedWidget(const FVector2D& ViewportPosition, UWidget* Widget)
{
	const FVector2D WidgetTopLeft = GetCachedWidgetPosition(Widget);
	const FVector2D WidgetBottomRight = GetCachedWidgetBottomRight(Widget);
	return ViewportPosition.ComponentwiseAllGreaterOrEqual(WidgetTopLeft) && ViewportPosition.ComponentwiseAllLessOrEqual(WidgetBottomRight);
}

FVector2D UBxWidgetUtils::GetClampedWidgetPosition(const UWidget* Widget, const FVector2D& ViewportPosition)
{
	FVector2D Result = ViewportPosition;
	const FVector2D& WidgetSize = Widget->GetDesiredSize();
	const FVector2D& ViewportSize = UWidgetLayoutLibrary::GetViewportSize(Widget);
	
	if (ViewportPosition.X + WidgetSize.X > ViewportSize.X)
	{
		Result.X = ViewportSize.X - WidgetSize.X;
	}
	
	Result.X = FMath::Max(Result.X, 0.f);
	
	if (ViewportPosition.Y + WidgetSize.Y > ViewportSize.Y)
	{
		Result.Y = ViewportSize.Y - WidgetSize.Y;
	}
	
	Result.Y = FMath::Max(Result.Y, 0.f);
	
	return Result;
}
