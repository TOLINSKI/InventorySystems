// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BxActorTracking.generated.h"

class UBxActorTrackingComponent;
class AActor;

struct FBxActorTrackingResult
{
	TWeakObjectPtr<AActor> ThisActor { nullptr };
	TWeakObjectPtr<AActor> LastActor { nullptr };
	bool bChanged { false };
};

/** Base Interface class for actor tracking strategies */
USTRUCT(BlueprintType)
struct BASEBOX_API FBxActorTracking
{
	GENERATED_BODY()

public:
	FBxActorTracking() {};
	
	FBxActorTracking(const FBxActorTracking&) = default; // Copy Constructor:
	FBxActorTracking& operator=(const FBxActorTracking&) = default; // Assignment Operator: 
	
	FBxActorTracking(FBxActorTracking&&) = default; // Move Constructor:
	FBxActorTracking& operator=(FBxActorTracking&&) = default; // Move Assignment Operator:  
	
	virtual ~FBxActorTracking() {}; // Virtual Destructor
	
	AActor* GetTrackedActor() const;

	/** 
	 * This function  must be overriden by child strategies
	 * 
	 * **ATTENTION**
	 * This function must return UpdateTrackingResult(AActor* HitActor)!!!
	 * 
	 * For example:
	 * FBxActorTrackingResult& TraceForActors(const UObject* WorldContextObject)
	 * {
	 *		..Do stuff..
	 *		AActor* HitActor = HitResult.GetActor();
	 *		return UpdateTrackingResult(HitActor).
	 * }
	 */
	virtual const FBxActorTrackingResult& TraceForActors(const UObject* WorldContextObject) { return UpdateTrackingResult(nullptr); }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	TEnumAsByte<ECollisionChannel> TraceChannel {ECC_WorldDynamic };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	TSubclassOf<UInterface> TraceInterface;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	bool bDrawDebug { false };
	
	FBxActorTrackingResult TrackingResult {};
	
	const FBxActorTrackingResult& UpdateTrackingResult(AActor* HitActor);
	
	bool IsActorRelevant(AActor* Actor);
	
private:
	bool bHasEverTrackedActor { false };
};
