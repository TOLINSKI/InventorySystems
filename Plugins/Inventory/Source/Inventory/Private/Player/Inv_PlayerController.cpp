// Copyright Chaos Box Games 2026, All rights reserved.


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"

class UEnhancedInputLocalPlayerSubsystem;

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (IsLocalPlayerController())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (!ensure(IsValid(Subsystem))) return;

		// Handle Mapping Contexts
		for (UInputMappingContext* MappingContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
		
		// Handle Input Actions
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Input_Interact);
	}
}

void AInv_PlayerController::Input_Interact()
{
	UE_LOG(LogInventory, Display, TEXT("Interact Input Pressed"));
}
