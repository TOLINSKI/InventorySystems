// Copyright Chaos Box Games 2026, All rights reserved.


#include "ActorTracking/Strategies/BxLineTracking.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

const FBxActorTrackingResult& FBxLineTracking::TraceForActors(const UObject* WorldContextObject)
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return TrackingResult;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	
	APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!ensure(IsValid(PlayerController))) return TrackingResult;
	
	FVector CameraCenter;
	FVector CameraDirection;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ViewportCenter, CameraCenter, CameraDirection);
	
	const FVector TraceEnd = CameraCenter + CameraDirection * TraceLength;
	FHitResult HitResult;
	PlayerController->GetWorld()->LineTraceSingleByChannel(HitResult, CameraCenter, TraceEnd, TraceChannel);
	AActor* HitActor = HitResult.GetActor();
	
	if (IsActorRelevant(HitActor))
	{
		DrawDebugPoint(PlayerController->GetWorld(), HitResult.ImpactPoint, 20.f, FColor::Green, false, -1.f, 1);
	}
	
	return UpdateTrackingResult(HitActor);
}

