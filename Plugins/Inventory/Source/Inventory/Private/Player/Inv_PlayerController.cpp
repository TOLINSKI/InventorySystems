// Copyright Chaos Box Games 2026, All rights reserved.


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/HUD/Inv_HUDWidget.h"
#include "Interaction/Inv_HighlightableStaticMeshComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "ActorTracking/Bx_ActorTrackingComponent.h"
#include "InventoryManagement/Inv_InventoryComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	ActorTracking = CreateDefaultSubobject<UBx_ActorTrackingComponent>("ActorTracker");
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupHUDWidget();
	ActorTracking->OnBeginTrackingActor.AddDynamic(this, &ThisClass::AInv_PlayerController::OnBeginTrackingActor);
	ActorTracking->OnEndTrackingActor.AddDynamic(this, &ThisClass::OnEndTrackingActor);
	
	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();
	ensure(InventoryComponent.IsValid());
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
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ThisClass::Input_ToggleInventory);
	}
}

void AInv_PlayerController::Input_Interact()
{
	AActor* Actor = ActorTracking->GetTrackedActor();
	if (!IsValid(Actor)) return;
	
	UInv_ItemComponent* ItemComponent = Actor->FindComponentByClass<UInv_ItemComponent>();
	if (!ensure(IsValid(ItemComponent))) return;
	
	InventoryComponent->TryAddItem(ItemComponent);
}

void AInv_PlayerController::Input_ToggleInventory()
{
	if (!ensure(InventoryComponent.IsValid())) return;
	
	InventoryComponent->ToggleInventoryMenu();
}

void AInv_PlayerController::SetupHUDWidget()
{
	if (!IsLocalPlayerController()) return;
	
	HUDWidget = Cast<UInv_HUDWidget>(CreateWidget(this, HUDWidgetClass));
	if (!ensure(HUDWidget)) return;
	
	HUDWidget->AddToViewport();
}

void AInv_PlayerController::OnBeginTrackingActor(AActor* Actor)
{
	UInv_ItemComponent* ItemComp = Actor->FindComponentByClass<UInv_ItemComponent>();
	if (!ensure(ItemComp)) return;
		
	HUDWidget->ShowItemMessage(ItemComp->GetPickupMessage());
	UActorComponent* Highlightable = Actor->FindComponentByInterface(UInv_Highlightable::StaticClass());
	if (!ensure(IsValid(Highlightable))) return;
		
	IInv_Highlightable::Execute_Highlight(Highlightable);
}

void AInv_PlayerController::OnEndTrackingActor(AActor* Actor)
{
	HUDWidget->HideItemMessage();
		
	UActorComponent* Highlightable = Actor->FindComponentByInterface(UInv_Highlightable::StaticClass());
	if (!ensure(IsValid(Highlightable))) return;
		
	IInv_Highlightable::Execute_UnHighlight(Highlightable);
}
