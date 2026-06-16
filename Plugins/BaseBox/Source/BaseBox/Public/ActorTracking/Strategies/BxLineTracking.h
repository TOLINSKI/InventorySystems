// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BxActorTracking.h"
#include "BxLineTracking.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct BASEBOX_API FBxLineTracking : public FBxActorTracking
{
	GENERATED_BODY()
	
protected:
	virtual const FBxActorTrackingResult& TraceForActors(const UObject* WorldContextObject) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	float TraceLength { 500.f };
};
