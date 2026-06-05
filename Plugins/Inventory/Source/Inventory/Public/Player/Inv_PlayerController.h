// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"

#include "Inv_PlayerController.generated.h"

class UInv_HUDWidget;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInv_PlayerController();

	virtual void Tick( float DeltaTime ) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Input_Interact();
	
	UPROPERTY(EditAnywhere, Category ="Inv Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	UPROPERTY(EditAnywhere, Category ="Inv Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category ="Inv UI")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category ="Inv Gameplay")
	TObjectPtr<class UInv_ActorTracker> ActorTracker;
	
private:
	void SetupHUDWidget();
	
	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;
	
	TWeakObjectPtr<AActor> CurrentHitActor;
	TWeakObjectPtr<AActor> LastHitActor;
	
	UFUNCTION()
	void OnBeginTrackingActor(AActor* Actor);
	
	UFUNCTION()
	void OnEndTrackingActor(AActor* Actor);
};
