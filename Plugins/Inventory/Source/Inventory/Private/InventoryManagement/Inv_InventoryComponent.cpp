// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/Inv_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!ensure(IsValidInventory())) return;
	
	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	OwningPlayer->SetInputMode(FInputModeGameAndUI());
	OwningPlayer->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!ensure(IsValidInventory())) return;
	
	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	OwningPlayer->SetInputMode(FInputModeGameOnly());
	if (bHideMouseCursorOnClosed) OwningPlayer->SetShowMouseCursor(false);
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (IsInventoryOpen())
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

bool UInv_InventoryComponent::IsInventoryOpen() const
{
	if (!ensure(IsValidInventory())) return false;
	return InventoryMenu->IsVisible();
}

bool UInv_InventoryComponent::IsValidInventory() const
{
	return OwningPlayer.IsValid() && IsValid(InventoryMenu);
}


void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ConstructInventory();
	CloseInventoryMenu();
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningPlayer = Cast<APlayerController>(GetOwner());
	if (!ensureMsgf(OwningPlayer.IsValid(), TEXT("Inventory Component must be owned by a Player Controller"))) return;
	
	InventoryMenu = CreateWidget<UInv_InventoryWidgetBase>(OwningPlayer.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
}



