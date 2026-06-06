// Copyright Chaos Box Games 2026, All rights reserved.


#include "Components/Base_ActorTrackingComponent.h"

#include "BaseBox.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


UBase_ActorTrackingComponent::UBase_ActorTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}


void UBase_ActorTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TraceForActors();
}

void UBase_ActorTrackingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayer = Cast<APlayerController>(GetOwner());
	ensure(OwningPlayer.IsValid());
}

AActor* UBase_ActorTrackingComponent::TraceForActors()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return nullptr;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	
	APlayerController* PlayerController = OwningPlayer.Get();
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
		LastHitLocation = HitResult.ImpactPoint;
		DrawDebugPoint(PlayerController->GetWorld(), HitResult.ImpactPoint, 25.f, FColor::Green, false, -1.f, 1);
	}
	
	if (CachedActor == HitActor)
	{
		return HitActor;
	}
	
	if (bIsHitActorRelevant)
	{
		OnBeginTrackingActor.Broadcast(HitActor);
		
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
		OnEndTrackingActor.Broadcast(CachedActor.Get());
		
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

