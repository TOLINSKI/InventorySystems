// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BxWidgetUtils.generated.h"

class UWidget;
/** Base Box Widget Utility Functions */
UCLASS()
class BASEBOX_API UBxWidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static int32 CoordinateToArrayIndex(const FIntPoint& Coordinate, const int32 NumColumns);

	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FIntPoint ArrayIndexToCoordinate(const int32 Index, const int32 NumColumns);
	
	/** Get Widget Top Left Corner in Viewport Coordinates */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetWidgetPosition(UWidget* Widget);
	
	/** Get Widget Center Position in Viewport Coordinates */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetWidgetCenter(UWidget* Widget);

	/** Get Widget Bottom Right Corner Position in Viewport Coordinates */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetWidgetBottomRight(UWidget* Widget);
	
	/** Returns true if viewport position is within widget geometry */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static bool IsPositionBoundByWidget(const FVector2D& ViewportPosition, UWidget* Widget);

	/** Returns true if Smaller Widget is within Bigger Widget geometry */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static bool IsWidgetBoundByWidget(UWidget* SmallerWidget, UWidget* BiggerWidget);

	/** 
	 * Gets the position that keeps the widget inside the viewport boundaries
	 * 
	 * NOTE: The underlying Slate widget must exist and be valid, also at least one pre-pass must have occurred before this value will be of any use.
	 * 
	 * @return The clamped position in viewport scale
	 */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetClampedWidgetPosition(const UWidget* Widget, const FVector2D& ViewportPosition);
};
