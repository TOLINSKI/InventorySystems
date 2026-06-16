// Copyright Chaos Box Games 2026, All rights reserved.


#include "ActorTracking/Strategies/BxActorTracking.h"

AActor* FBxActorTracking::GetTrackedActor() const
{
	return TrackingResult.ThisActor.Get();
}

const FBxActorTrackingResult& FBxActorTracking::UpdateTrackingResult(AActor* HitActor)
{
	TrackingResult.LastActor = TrackingResult.ThisActor;
	TrackingResult.ThisActor = IsActorRelevant(HitActor) ? HitActor : nullptr;
	TrackingResult.bChanged = (TrackingResult.LastActor != TrackingResult.ThisActor) || bHasEverTrackedActor && !TrackingResult.ThisActor.IsValid();
	
	bHasEverTrackedActor = TrackingResult.ThisActor.IsValid();
	
	return TrackingResult;
}

bool FBxActorTracking::IsActorRelevant(AActor* Actor)
{
	return IsValid(Actor) && (IsValid(TraceInterface) ? Actor->GetClass()->ImplementsInterface(TraceInterface) : true);
}
