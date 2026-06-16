// Copyright Chaos Box Games 2026, All rights reserved.


#include "ActorTracking/Strategies/BxSphereTracking.h"

#include "Engine/Engine.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

const FBxActorTrackingResult& FBxSphereTracking::TraceForActors(const UObject* WorldContextObject)
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return TrackingResult;
	
	UWorld* World = WorldContextObject->GetWorld();
	check(IsValid(World));
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(IsValid(PlayerController))) return TrackingResult;
	
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!ensure(IsValid(PlayerPawn))) return TrackingResult;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerPawn);
	
	TArray<FOverlapResult> Overlaps;
	PlayerController->GetWorld()->OverlapMultiByChannel(
		Overlaps, 
		PlayerPawn->GetActorLocation(), 
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(TraceRadius),
		Params);
	
	TArray<AActor*> OverlappedActors;
	OverlappedActors.Reserve(Overlaps.Num());
	for (auto Overlap : Overlaps)
	{
		if (!IsValid(Overlap.GetActor())) continue;
		OverlappedActors.Add(Overlap.GetActor());
	}
	
	float DistanceToNearest {0.f};
	AActor* HitActor = UGameplayStatics::FindNearestActor(PlayerPawn->GetActorLocation(), OverlappedActors, DistanceToNearest);
	
	if (bDrawDebug)
	{
		if (TrackingResult.ThisActor.IsValid())
		{
			DrawDebugSphere(World, PlayerPawn->GetActorLocation(), TraceRadius, 24, FColor::Green);
			DrawDebugPoint(PlayerController->GetWorld(), TrackingResult.ThisActor->GetActorLocation(), 25.f, FColor::Yellow, false, -1.f, 1);
		}
		else
		{
			DrawDebugSphere(World, PlayerPawn->GetActorLocation(), TraceRadius, 24, FColor::White);
		}
	}

	return UpdateTrackingResult(HitActor);
}
