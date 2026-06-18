// Copyright Chaos Box Games 2026, All rights reserved.


#include "BaseBoxFunctionLibrary.h"


int32 UBaseBoxFunctionLibrary::CoordinateToArrayIndex(const FIntPoint& Coordinate, const int32 NumColumns)
{
	return Coordinate.Y * NumColumns + Coordinate.X;
}

FIntPoint UBaseBoxFunctionLibrary::ArrayIndexToCoordinate(const int32 Index, const int32 NumColumns)
{
	return {Index % NumColumns, Index / NumColumns};
}