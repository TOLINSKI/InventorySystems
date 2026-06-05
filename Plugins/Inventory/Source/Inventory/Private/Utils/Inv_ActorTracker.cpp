// Copyright Chaos Box Games 2026, All rights reserved.


#include "Inv_ActorTracker.h"

#include "Inventory.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

AActor* UInv_ActorTracker::TraceForActors(APlayerController* PlayerController)
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return nullptr;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector CameraCenter;
	FVector CameraDirection;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ViewportCenter, CameraCenter, CameraDirection);
	
	const FVector TraceEnd = CameraCenter + CameraDirection * TraceLength;
	FHitResult HitResult;
	PlayerController->GetWorld()->LineTraceSingleByChannel(HitResult, CameraCenter, TraceEnd, TraceChannel);
	
	AActor* HitActor = HitResult.GetActor();
	
	if (CachedActor == HitActor)
	{
		return HitActor;
	}
	
	if (IsValid(HitActor))
	{
		UE_LOG(LogInventory, Display, TEXT("Item Trace Started Hitting Actor: %s"), *HitActor->GetName());
		OnBeginTrackingActor.Broadcast(HitActor);
	}
	
	if (CachedActor.IsValid())
	{
		UE_LOG(LogInventory, Display, TEXT("Item Trace Stopped Hitting Actor: %s"), *CachedActor->GetName());
		OnEndTrackingActor.Broadcast(HitActor);
	}

	CachedActor = HitActor;
	return HitActor;
}