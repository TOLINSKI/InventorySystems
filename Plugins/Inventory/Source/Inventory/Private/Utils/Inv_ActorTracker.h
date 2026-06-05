// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Engine/EngineTypes.h"

#include "Inv_ActorTracker.generated.h"

class AActor;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorTrackerDelegate, AActor*, HitActor);

/**
 * Caches the last hit actor
 * 
 * Useful for tracking a hit actor
 */
UCLASS(NotBlueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class INVENTORY_API UInv_ActorTracker : public UObject
{
	GENERATED_BODY()
	
public:
	UInv_ActorTracker() = default;

	AActor* GetHitActor() const { return CachedActor.Get(); }
	AActor* TraceForActors(APlayerController* PlayerController);

	FActorTrackerDelegate OnBeginTrackingActor;
	FActorTrackerDelegate OnEndTrackingActor;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inv Item Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel {ECC_WorldDynamic };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inv Item Trace")
	float TraceLength { 500.f };
	
private:
	TWeakObjectPtr<AActor> CachedActor;
};
