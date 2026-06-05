// Copyright Chaos Box Games 2026, All rights reserved.


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Inv_HUDWidget.h"
#include "Engine/Engine.h"
#include "Interaction/Inv_HighlightableStaticMeshComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Utils/Inv_ActorTracker.h"

class UEnhancedInputLocalPlayerSubsystem;

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupHUDWidget();
	ActorTracker->OnBeginTrackingActor.AddDynamic(this, &ThisClass::AInv_PlayerController::OnBeginTrackingActor);
	ActorTracker->OnEndTrackingActor.AddDynamic(this, &ThisClass::OnEndTrackingActor);
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

void AInv_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AActor* HitActor = ActorTracker->TraceForActors(this);
}

void AInv_PlayerController::Input_Interact()
{
	UE_LOG(LogInventory, Display, TEXT("Interact Input Pressed"));
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
		
	HUDWidget->ShowPickupMessage(ItemComp->GetPickupMessage());
	UActorComponent* Highlightable = Actor->FindComponentByInterface(UInv_HighlightableStaticMeshComponent::StaticClass());
	if (!ensure(IsValid(Highlightable))) return;
		
	IInv_Highlightable::Execute_Highlight(Highlightable);
}

void AInv_PlayerController::OnEndTrackingActor(AActor* Actor)
{
	HUDWidget->HidePickupMessage();
		
	UActorComponent* Highlightable = Actor->FindComponentByInterface(UInv_HighlightableStaticMeshComponent::StaticClass());
	if (!ensure(IsValid(Highlightable))) return;
		
	IInv_Highlightable::Execute_UnHighlight(Highlightable);
}
