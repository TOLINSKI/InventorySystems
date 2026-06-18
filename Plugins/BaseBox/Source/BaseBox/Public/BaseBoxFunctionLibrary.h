// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseBoxFunctionLibrary.generated.h"

class UBxCompositeContainer;
class UBxComposite;
/**
 * 
 */
UCLASS()
class BASEBOX_API UBaseBoxFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/** Flatten a 2D coordinate to an array index */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static int32 CoordinateToArrayIndex(const FIntPoint& Coordinate, const int32 NumColumns);

	/** Convert an array index to a 2D coordinate */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FIntPoint ArrayIndexToCoordinate(const int32 Index, const int32 NumColumns);
};
