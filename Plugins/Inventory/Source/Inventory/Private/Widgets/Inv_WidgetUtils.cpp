// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Inv_WidgetUtils.h"

int32 UInv_WidgetUtils::GridPositionToArrayIndex(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}
