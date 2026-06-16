// Copyright Chaos Box Games 2026, All rights reserved.


#include "ActorTracking/BxActorTrackingComponent.h"

#include "BaseBox.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorTracking/Strategies/BxActorTracking.h"


UBxActorTrackingComponent::UBxActorTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	
	bDrawDebug = false;
}

void UBxActorTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!OwningPlayer.IsValid()) return;
	
	const FBxActorTrackingResult& Result = GetMutableActorTracking()->TraceForActors(this);
	
	if (!Result.bChanged) return;
	
	if (!Result.ThisActor.IsValid())
	{
		OnEndTrackingActor.Broadcast(nullptr);
		
		if (bDrawDebug)
		{
			const FString Msg = FString::Printf(TEXT("End Tacking Actor: NULL"));
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *Msg));
			UE_LOG(LogBaseBoxPlugin, Display, TEXT("%s"), *Msg);
		}
	}
	
	if (Result.ThisActor.IsValid())
	{
		OnBeginTrackingActor.Broadcast(Result.ThisActor.Get());
		
		if (bDrawDebug)
		{
			const FString Msg = FString::Printf(TEXT("Begin Tracking Actor: %s"), *Result.ThisActor->GetActorNameOrLabel());
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *Msg));
			UE_LOG(LogBaseBoxPlugin, Display, TEXT("%s"), *Msg);
		}
	}
	
	if (Result.LastActor.IsValid())
	{
		OnEndTrackingActor.Broadcast(Result.LastActor.Get());
		
		if (bDrawDebug)
		{
			const FString Msg = FString::Printf(TEXT("End Tracking Actor: %s"), *Result.LastActor->GetActorNameOrLabel());
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *Msg));
			UE_LOG(LogBaseBoxPlugin, Display, TEXT("%s"), *Msg);
		}
	}
}

AActor* UBxActorTrackingComponent::GetTrackedActor() const
{
	return GetActorTracking()->GetTrackedActor();
}

void UBxActorTrackingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayer = Cast<APlayerController>(GetOwner());
	ensureMsgf(OwningPlayer.IsValid(), TEXT("Actor Tracking Component must be owned by a player controller."));
}

FBxActorTracking* UBxActorTrackingComponent::GetMutableActorTracking()
{
	return ActorTracking.GetMutablePtr<FBxActorTracking>();
}

const FBxActorTracking* UBxActorTrackingComponent::GetActorTracking() const
{
	return ActorTracking.GetPtr<FBxActorTracking>();
}
