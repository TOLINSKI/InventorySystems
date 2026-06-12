// Copyright Chaos Box Games 2026, All rights reserved.


#include "ActorTracking/Strategies/Bx_LineTraceTracking.h"

#include "BaseBox.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AActor* UBx_LineTraceTracking::TraceForActors()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return nullptr;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(IsValid(PlayerController))) return nullptr;
	
	FVector CameraCenter;
	FVector CameraDirection;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ViewportCenter, CameraCenter, CameraDirection);
	
	const FVector TraceEnd = CameraCenter + CameraDirection * TraceLength;
	FHitResult HitResult;
	PlayerController->GetWorld()->LineTraceSingleByChannel(HitResult, CameraCenter, TraceEnd, TraceChannel);
	
	AActor* HitActor = HitResult.GetActor();
	bool bIsHitActorRelevant = IsValid(HitActor) && (IsValid(TraceInterface) ? HitActor->GetClass()->ImplementsInterface(TraceInterface) : true); 
	
	if (CachedActor.IsValid() && bDrawDebug)
	{
		DrawDebugPoint(PlayerController->GetWorld(), HitResult.ImpactPoint, 20.f, FColor::Green, false, -1.f, 1);
	}
	
	if (CachedActor == HitActor)
	{
		return HitActor;
	}
	
	if (bIsHitActorRelevant)
	{
		HitActor->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnCachedActorDestroyed);
		
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
		CachedActor->OnDestroyed.RemoveAll(this);
		
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

void UBx_LineTraceTracking::OnCachedActorDestroyed(AActor* Actor)
{
	OnEndTrackingActor.ExecuteIfBound(Actor);
		
	if (bDrawDebug)
	{
		const FString Msg = FString::Printf(TEXT("Actor Trace Stopped Hitting: %s"), *Actor->GetActorNameOrLabel());
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *Msg));
		UE_LOG(LogBaseBoxPlugin, Display, TEXT("%s"), *Msg);
	}
}

