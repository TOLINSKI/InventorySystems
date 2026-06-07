// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Bx_ActorTrackingStrategy.generated.h"

DECLARE_DELEGATE_OneParam(FBxActorTrackingEvent, AActor* /* Tracked Actor */)

/**
 * 
 */
UCLASS(Abstract, BlueprintType, EditInlineNew, meta = (DefaultToInstanced))
class BASEBOX_API UBx_ActorTrackingStrategy : public UObject
{
	GENERATED_BODY()
	
	friend class UBx_ActorTrackingComponent;
	
public:
	UFUNCTION(BlueprintCallable, Category="Tracking")
	AActor* GetTrackedActor() const { return CachedActor.Get(); }
	
	FBxActorTrackingEvent OnBeginTrackingActor;
	FBxActorTrackingEvent OnEndTrackingActor;
	
protected:
	virtual AActor* TraceForActors() { return nullptr;};
	
	TWeakObjectPtr<AActor> CachedActor;
};
