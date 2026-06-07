// Copyright Chaos Box Games 2026, All rights reserved.


#include "ActorTracking/Bx_ActorTrackingComponent.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorTracking/Strategies/Bx_ActorTrackingStrategy.h"


UBx_ActorTrackingComponent::UBx_ActorTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBx_ActorTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TrackingStrategy->TraceForActors();
}

AActor* UBx_ActorTrackingComponent::GetTrackedActor() const
{
	return TrackingStrategy->GetTrackedActor();
}

void UBx_ActorTrackingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayer = Cast<APlayerController>(GetOwner());
	ensure(OwningPlayer.IsValid());
	
	TrackingStrategy->OnBeginTrackingActor.BindLambda([this](AActor* Actor)
	{
		OnBeginTrackingActor.Broadcast(Actor);
	});
	
	TrackingStrategy->OnEndTrackingActor.BindLambda([this](AActor* Actor)
	{
		OnEndTrackingActor.Broadcast(Actor);
	});
}
