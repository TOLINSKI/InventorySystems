// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

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

FVector2D UInv_WidgetUtils::GetClampedWidgetPosition(const UWidget* Widget, const FVector2D& ViewportPosition)
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

