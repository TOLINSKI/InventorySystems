// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "BxActorTrackingComponent.generated.h"

class AActor;
class APlayerController;
struct FBxActorTracking;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorTrackingDelegate, AActor*, HitActor);

UCLASS(ClassGroup=(BaseBox), meta=(BlueprintSpawnableComponent))
class BASEBOX_API UBxActorTrackingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBxActorTrackingComponent();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintAssignable, Category="Events|Actor Tracking")
	FActorTrackingDelegate OnBeginTrackingActor;
	
	UPROPERTY(BlueprintAssignable, Category="Events|Actor Tracking")
	FActorTrackingDelegate OnEndTrackingActor;
	
	AActor* GetTrackedActor() const;
	
protected:
	UPROPERTY(EditAnywhere, Category="Actor Tracking", meta = (ExcludeBaseStruct))
	TInstancedStruct<FBxActorTracking> ActorTracking;	
	
	UPROPERTY(EditAnywhere, Category="Actor Tracking", meta = (ExcludeBaseStruct))
	bool bDrawDebug;
	
private:
	TWeakObjectPtr<APlayerController> OwningPlayer;
	
	FBxActorTracking* GetMutableActorTracking();
	
	const FBxActorTracking* GetActorTracking() const;
};
