// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/BxWidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

int32 UBxWidgetUtils::CoordinateToArrayIndex(const FIntPoint& Coordinate, const int32 NumColumns)
{
	return Coordinate.Y * NumColumns + Coordinate.X;
}

FIntPoint UBxWidgetUtils::ArrayIndexToCoordinate(const int32 Index, const int32 NumColumns)
{
	return {Index % NumColumns, Index / NumColumns};
}

FVector2D UBxWidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsolutePosition, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UBxWidgetUtils::GetWidgetCenter(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	const FVector2D AbsoluteSize = Widget->GetCachedGeometry().GetAbsoluteSize();
	const FVector2D AbsoluteCenter = AbsolutePosition + AbsoluteSize / 2.f;
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsoluteCenter, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UBxWidgetUtils::GetWidgetBottomRight(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	const FVector2D AbsoluteSize = Widget->GetCachedGeometry().GetAbsoluteSize();
	const FVector2D AbsoluteBottomRight = AbsolutePosition + AbsoluteSize;
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsoluteBottomRight, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

bool UBxWidgetUtils::IsPositionBoundByWidget(const FVector2D& ViewportPosition, UWidget* Widget)
{
	const FVector2D WidgetTopLeft = GetWidgetPosition(Widget);
	const FVector2D WidgetBottomRight = GetWidgetBottomRight(Widget);
	return ViewportPosition.ComponentwiseAllGreaterOrEqual(WidgetTopLeft) && ViewportPosition.ComponentwiseAllLessOrEqual(WidgetBottomRight);
}

bool UBxWidgetUtils::IsWidgetBoundByWidget(UWidget* SmallerWidget, UWidget* BiggerWidget)
{
	const FVector2D SmallerWidgetTopLeft = GetWidgetPosition(SmallerWidget);
	const FVector2D SmallerWidgetBottomRight = GetWidgetBottomRight(SmallerWidget);
	const FVector2D BiggerWidgetTopLeft = GetWidgetPosition(BiggerWidget);
	const FVector2D BiggerWidgetBottomRight = GetWidgetBottomRight(BiggerWidget);
	return SmallerWidgetTopLeft.ComponentwiseAllGreaterOrEqual(BiggerWidgetTopLeft) && SmallerWidgetBottomRight.ComponentwiseAllLessOrEqual(BiggerWidgetBottomRight);
}
