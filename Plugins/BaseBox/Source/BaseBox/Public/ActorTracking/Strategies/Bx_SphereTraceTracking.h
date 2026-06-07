// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bx_ActorTrackingStrategy.h"
#include "Bx_SphereTraceTracking.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, meta = (DefaultToInstanced))
class BASEBOX_API UBx_SphereTraceTracking : public UBx_ActorTrackingStrategy
{
	GENERATED_BODY()
	
protected:
	virtual AActor* TraceForActors() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	TEnumAsByte<ECollisionChannel> TraceChannel {ECC_WorldDynamic };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	TSubclassOf<UInterface> TraceInterface;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	float TraceRadius { 150.f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tracking")
	bool bDrawDebug { false };
};
