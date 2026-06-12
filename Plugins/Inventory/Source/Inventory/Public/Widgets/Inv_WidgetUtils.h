// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_WidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static int32 GridPositionToIndex(const FIntPoint& Position, const int32 Columns);
	static FIntPoint IndexToGridPosition(const int32 Index, const int32 Columns);
	static TSet<int32> GetOccupiedIndices(const int32 Index, const FIntPoint& Range2D, const int32 Columns);
	static FVector2D GetWidgetPosition(UUserWidget* Widget);
	static FVector2D GetWidgetCenter(UUserWidget* Widget);
	
	template<typename T, typename Predicate>
	static void ForEach2D(TArray<T>& Array, const int32 Index, const FIntPoint& Range2D, int32 Columns, const Predicate& Pred);
	
	template<typename T, typename Predicate>
	static void ForEach2D(const TArray<T>& Array, const int32 Index, const FIntPoint& Range2D, int32 Columns, const Predicate& Pred);
};

template <typename T, typename Predicate>
void UInv_WidgetUtils::ForEach2D(TArray<T>& Array, const int32 Index, const FIntPoint& Range2D, int32 Columns,
	const Predicate& Pred)
{
	const FIntPoint Position2D = IndexToGridPosition(Index, Columns);
	
	for (int32 Row = Position2D.Y; Row < Position2D.Y + Range2D.Y; ++Row)
	{
		for (int32 Column = Position2D.X; Column < Position2D.X + Range2D.X; ++Column)
		{
			const int32 ArrayIndex = GridPositionToIndex({Column, Row}, Columns);
			if (Array.IsValidIndex(ArrayIndex))
			{
				Pred(Array[ArrayIndex]);
			}
		}
	}
}

template <typename T, typename Predicate>
void UInv_WidgetUtils::ForEach2D(const TArray<T>& Array, const int32 Index, const FIntPoint& Range2D, int32 Columns,
	const Predicate& Pred)
{
	const FIntPoint Position2D = IndexToGridPosition(Index, Columns);
	
	for (int32 Row = Position2D.Y; Row < Position2D.Y + Range2D.Y; ++Row)
	{
		for (int32 Column = Position2D.X; Column < Position2D.X + Range2D.X; ++Column)
		{
			const int32 ArrayIndex = GridPositionToIndex({Column, Row}, Columns);
			if (Array.IsValidIndex(ArrayIndex))
			{
				Pred(Array[ArrayIndex]);
			}
		}
	}
}