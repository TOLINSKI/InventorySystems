// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BxActorTracking.h"
#include "BxSphereOverlapTracking.generated.h"

/** Track actors using a sphere overlap test around the player pawn */
USTRUCT(BlueprintType, DisplayName = "Sphere Overlap Tracking")
struct BASEBOX_API FBxSphereOverlapTracking : public FBxActorTracking
{
	GENERATED_BODY()

public:
	virtual const FBxActorTrackingResult& TraceForActors(const UObject* WorldContextObject) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	float TraceRadius { 150.f };
};
