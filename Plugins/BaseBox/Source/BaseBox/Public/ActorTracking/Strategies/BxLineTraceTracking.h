// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BxActorTracking.h"
#include "BxLineTraceTracking.generated.h"

/** Track actors using a line trace from camera center */
USTRUCT(BlueprintType, DisplayName = "Line Trace Tracking")
struct BASEBOX_API FBxLineTraceTracking : public FBxActorTracking
{
	GENERATED_BODY()
	
public:
	virtual const FBxActorTrackingResult& TraceForActors(const UObject* WorldContextObject) override;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	float TraceLength { 500.f };
};
