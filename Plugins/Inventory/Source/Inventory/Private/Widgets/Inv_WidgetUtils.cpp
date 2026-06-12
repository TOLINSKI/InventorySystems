// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

int32 UInv_WidgetUtils::GridPositionToIndex(const FIntPoint& Position, const int32 Columns)
{
	return Position.Y * Columns + Position.X;
}

FIntPoint UInv_WidgetUtils::IndexToGridPosition(const int32 Index, const int32 Columns)
{
	return {Index % Columns, Index / Columns};
}

TSet<int32> UInv_WidgetUtils::GetOccupiedIndices(const int32 Index, const FIntPoint& Range2D, const int32 Columns)
{
	TSet<int32> Indices;
	Indices.Add(Index);
	
	const FIntPoint GridPosition = IndexToGridPosition(Index, Columns);
	for (int32 i = 1; i < Range2D.Y; ++i)
	{
		for (int32 j = 1; j < Range2D.X; ++j)
		{
			const FIntPoint ThisGridPosition = GridPosition + FIntPoint(i, j);
			Indices.Add(GridPositionToIndex(ThisGridPosition, Columns));
		}
	}
	
	return Indices;
}

FVector2D UInv_WidgetUtils::GetWidgetPosition(UUserWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetWidgetCenter(UUserWidget* Widget)
{
	FVector2D ViewportTopLeft = GetWidgetPosition(Widget);
	const FGeometry Geometry = Widget->GetCachedGeometry();
	const FVector2D WidgetSize = USlateBlueprintLibrary::GetLocalSize(Geometry);
	const FVector2D WidgetCenter = ViewportTopLeft + WidgetSize / 2.f;
	return WidgetCenter;
}

