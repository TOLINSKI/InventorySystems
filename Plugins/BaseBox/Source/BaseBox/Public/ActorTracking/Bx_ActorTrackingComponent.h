// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Bx_ActorTrackingComponent.generated.h"

class UBx_ActorTrackingStrategy;
class AActor;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorTrackingDelegate, AActor*, HitActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASEBOX_API UBx_ActorTrackingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBx_ActorTrackingComponent();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	FActorTrackingDelegate OnBeginTrackingActor;
	FActorTrackingDelegate OnEndTrackingActor;
	
	AActor* GetTrackedActor() const;
	
protected:
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite, Category="Tracking")
	TObjectPtr<UBx_ActorTrackingStrategy> TrackingStrategy;	
	
private:
	TWeakObjectPtr<APlayerController> OwningPlayer;
};
