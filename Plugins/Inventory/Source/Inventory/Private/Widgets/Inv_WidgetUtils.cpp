// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

int32 UInv_WidgetUtils::GridCoordinatesToIndex(const FIntPoint& Position, const int32 Columns)
{
	return Position.Y * Columns + Position.X;
}

FIntPoint UInv_WidgetUtils::IndexToGridCoordinates(const int32 Index, const int32 Columns)
{
	return {Index % Columns, Index / Columns};
}

TSet<int32> UInv_WidgetUtils::GetOccupiedIndices(const int32 Index, const FIntPoint& Range2D, const int32 Columns)
{
	TSet<int32> Indices;
	Indices.Add(Index);
	
	const FIntPoint GridCoordinates = IndexToGridCoordinates(Index, Columns);
	for (int32 i = 1; i < Range2D.Y; ++i)
	{
		for (int32 j = 1; j < Range2D.X; ++j)
		{
			const FIntPoint ThisGridCoordinates = GridCoordinates + FIntPoint(i, j);
			Indices.Add(GridCoordinatesToIndex(ThisGridCoordinates, Columns));
		}
	}
	
	return Indices;
}

FVector2D UInv_WidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsolutePosition, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetWidgetCenter(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	const FVector2D AbsoluteSize = Widget->GetCachedGeometry().GetAbsoluteSize();
	const FVector2D AbsoluteCenter = AbsolutePosition + AbsoluteSize / 2.f;
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsoluteCenter, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetWidgetBottomRight(UWidget* Widget)
{
	const FVector2D AbsolutePosition = Widget->GetCachedGeometry().GetAbsolutePosition();
	const FVector2D AbsoluteSize = Widget->GetCachedGeometry().GetAbsoluteSize();
	const FVector2D AbsoluteBottomRight = AbsolutePosition + AbsoluteSize;
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(Widget->GetOwningPlayer(), AbsoluteBottomRight, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetClampedWidgetPosition(const UWidget* Widget, const FVector2D& ViewportPosition,
                                                     const FVector2D& Boundary)
{
	FVector2D Result = ViewportPosition;
	const FVector2D& WidgetSize = Widget->GetDesiredSize();
	
	if (ViewportPosition.X + WidgetSize.X > Boundary.X)
	{
		Result.X = Boundary.X - WidgetSize.X;
	}
	
	Result.X = FMath::Max(Result.X, 0.f);
	
	if (ViewportPosition.Y + WidgetSize.Y > Boundary.Y)
	{
		Result.Y = Boundary.Y - WidgetSize.Y;
	}
	
	Result.Y = FMath::Max(Result.Y, 0.f);
	
	return Result;
}

bool UInv_WidgetUtils::IsPositionBoundByWidget(UWidget* Widget, const FVector2D& ViewportPosition)
{
	const FVector2D WidgetTopLeft = GetWidgetPosition(Widget);
	const FVector2D WidgetBottomRight = GetWidgetBottomRight(Widget);
	return ViewportPosition.ComponentwiseAllGreaterOrEqual(WidgetTopLeft) && ViewportPosition.ComponentwiseAllLessOrEqual(WidgetBottomRight);
}

bool UInv_WidgetUtils::IsWidgetBoundByWidget(UWidget* BiggerWidget, UWidget* SmallerWidget)
{
	const FVector2D SmallerWidgetTopLeft = GetWidgetPosition(SmallerWidget);
	const FVector2D SmallerWidgetBottomRight = GetWidgetBottomRight(SmallerWidget);
	const FVector2D BiggerWidgetTopLeft = GetWidgetPosition(BiggerWidget);
	const FVector2D BiggerWidgetBottomRight = GetWidgetBottomRight(BiggerWidget);
	return SmallerWidgetTopLeft.ComponentwiseAllGreaterOrEqual(BiggerWidgetTopLeft) && SmallerWidgetBottomRight.ComponentwiseAllLessOrEqual(BiggerWidgetBottomRight);
}

