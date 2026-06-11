// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inv_WidgetUtils.h"

int32 UInv_WidgetUtils::GridPositionToIndex(const FIntPoint& Position, const int32 Columns)
{
	return Position.Y * Columns + Position.X;
}

FIntPoint UInv_WidgetUtils::IndexToGridPosition(const int32 Index, const int32 Columns)
{
	return {Index % Columns, Index / Columns};
}
