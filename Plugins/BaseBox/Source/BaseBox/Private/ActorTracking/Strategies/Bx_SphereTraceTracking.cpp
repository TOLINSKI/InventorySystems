// Copyright Chaos Box Games 2026, All rights reserved.


#include "ActorTracking/Strategies/Bx_SphereTraceTracking.h"

#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "BaseBox.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AActor* UBx_SphereTraceTracking::TraceForActors()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return nullptr;
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(IsValid(PlayerController))) return nullptr;
	
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!ensure(IsValid(PlayerPawn))) return nullptr;
	
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
	bool bIsHitActorRelevant = IsValid(HitActor) && (IsValid(TraceInterface) ? HitActor->GetClass()->ImplementsInterface(TraceInterface) : true); 
	
	if (bDrawDebug)
	{
		if (CachedActor.IsValid())
		{
			DrawDebugSphere(GetWorld(), PlayerPawn->GetActorLocation(), TraceRadius, 24, FColor::Green);
			DrawDebugPoint(PlayerController->GetWorld(), CachedActor->GetActorLocation(), 25.f, FColor::Yellow, false, -1.f, 1);
		}
		else
		{
			DrawDebugSphere(GetWorld(), PlayerPawn->GetActorLocation(), TraceRadius, 24, FColor::White);
		}
	}
	
	if (CachedActor == HitActor)
	{
		return HitActor;
	}
	
	if (bIsHitActorRelevant)
	{
		OnBeginTrackingActor.ExecuteIfBound(HitActor);
		
		if (bDrawDebug)
		{
			const FString Msg = FString::Printf(TEXT("Actor Trace Started Hitting: %s"), *HitActor->GetActorNameOrLabel());
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *Msg));
			UE_LOG(LogBaseBoxPlugin, Display, TEXT("%s"), *Msg);
		}
	}
	
	if (CachedActor.IsValid())
	{
		OnEndTrackingActor.ExecuteIfBound(CachedActor.Get());
		
		if (bDrawDebug)
		{
			const FString Msg = FString::Printf(TEXT("Actor Trace Stopped Hitting: %s"), *CachedActor->GetActorNameOrLabel());
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *Msg));
			UE_LOG(LogBaseBoxPlugin, Display, TEXT("%s"), *Msg);
		}
	}

	CachedActor = HitActor;
	return HitActor;
}
