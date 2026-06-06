// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Base_ActorTrackingComponent.generated.h"

class AActor;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBsb_ActorTrackerDelegate, AActor*, HitActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASEBOX_API UBase_ActorTrackingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBase_ActorTrackingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	
public:
	AActor* GetHitActor() const { return CachedActor.Get(); }
	AActor* TraceForActors();

	FBsb_ActorTrackerDelegate OnBeginTrackingActor;
	FBsb_ActorTrackerDelegate OnEndTrackingActor;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TEnumAsByte<ECollisionChannel> TraceChannel {ECC_WorldDynamic };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<UInterface> TraceInterface;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	float TraceLength { 500.f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	bool bDrawDebug;
	
private:
	TWeakObjectPtr<AActor> CachedActor;
	
	TWeakObjectPtr<APlayerController> OwningPlayer;
	
	FVector LastHitLocation;
};
