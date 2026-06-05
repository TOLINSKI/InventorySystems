// Copyright Chaos Box Games 2026, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Input_Interact();
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> InteractAction;
};
