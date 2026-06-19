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
	/** 
	 * Get Widget Top Left Corner in Viewport Coordinates 
	 * 
	 * EPIC NOTE:
	 * The underlying Slate widget must exist and be valid, also at least one pre-pass must have occurred before this value will be of any use.
	 * Gets the last geometry used to Tick the widget.  This data may not exist yet if this call happens prior to 
	 * the widget having been ticked/painted, or it may be out of date, or a frame behind.
	 *
	 * We recommend not to use this data unless there's no other way to solve your problem.  Normally in Slate we
	 * try and handle these issues by making a dependent widget part of the hierarchy, as to avoid frame behind
	 * or what are referred to as hysteresis problems, both caused by depending on geometry from the previous frame
	 * being used to advise how to layout a dependent object the current frame.
	 */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetCachedWidgetPosition(UWidget* Widget);
	
	/** 
	 * Get Widget Center Position in Viewport Coordinates 
	 * 
	 * EPIC NOTE:
	 * The underlying Slate widget must exist and be valid, also at least one pre-pass must have occurred before this value will be of any use.
	 * Gets the last geometry used to Tick the widget.  This data may not exist yet if this call happens prior to 
	 * the widget having been ticked/painted, or it may be out of date, or a frame behind.
	 *
	 * We recommend not to use this data unless there's no other way to solve your problem.  Normally in Slate we
	 * try and handle these issues by making a dependent widget part of the hierarchy, as to avoid frame behind
	 * or what are referred to as hysteresis problems, both caused by depending on geometry from the previous frame
	 * being used to advise how to layout a dependent object the current frame.
	 */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetCachedWidgetCenter(UWidget* Widget);

	/** 
	 * Get Widget Bottom Right Corner Position in Viewport Coordinates 
	 * 
	 * EPIC NOTE:
	 * The underlying Slate widget must exist and be valid, also at least one pre-pass must have occurred before this value will be of any use.
	 * Gets the last geometry used to Tick the widget.  This data may not exist yet if this call happens prior to 
	 * the widget having been ticked/painted, or it may be out of date, or a frame behind.
	 *
	 * We recommend not to use this data unless there's no other way to solve your problem.  Normally in Slate we
	 * try and handle these issues by making a dependent widget part of the hierarchy, as to avoid frame behind
	 * or what are referred to as hysteresis problems, both caused by depending on geometry from the previous frame
	 * being used to advise how to layout a dependent object the current frame.
	 */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetCachedWidgetBottomRight(UWidget* Widget);
	
	/** 
	 * Returns true if viewport position is within widget geometry 
	 * 
	 * EPIC NOTE:
	 * The underlying Slate widget must exist and be valid, also at least one pre-pass must have occurred before this value will be of any use.
	 * Gets the last geometry used to Tick the widget.  This data may not exist yet if this call happens prior to 
	 * the widget having been ticked/painted, or it may be out of date, or a frame behind.
	 *
	 * We recommend not to use this data unless there's no other way to solve your problem.  Normally in Slate we
	 * try and handle these issues by making a dependent widget part of the hierarchy, as to avoid frame behind
	 * or what are referred to as hysteresis problems, both caused by depending on geometry from the previous frame
	 * being used to advise how to layout a dependent object the current frame.
	 */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static bool IsPositionBoundByCachedWidget(const FVector2D& ViewportPosition, UWidget* Widget);

	/** 
	 * Gets the position that keeps the widget inside the viewport boundaries
	 * 
	 * EPIC NOTE:
	 * The underlying Slate widget must exist and be valid, also at least one pre-pass must have occurred before this value will be of any use.
	 * Gets the last geometry used to Tick the widget.  This data may not exist yet if this call happens prior to 
	 * the widget having been ticked/painted, or it may be out of date, or a frame behind.
	 *
	 * We recommend not to use this data unless there's no other way to solve your problem.  Normally in Slate we
	 * try and handle these issues by making a dependent widget part of the hierarchy, as to avoid frame behind
	 * or what are referred to as hysteresis problems, both caused by depending on geometry from the previous frame
	 * being used to advise how to layout a dependent object the current frame.
	 * 
	 * @return The clamped position in viewport scale
	 */
	UFUNCTION(BlueprintPure, Category="Inventory Widget Utils")
	static FVector2D GetClampedWidgetPosition(const UWidget* Widget, const FVector2D& ViewportPosition);
};
