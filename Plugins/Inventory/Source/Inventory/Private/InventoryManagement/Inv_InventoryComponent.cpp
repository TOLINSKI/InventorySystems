// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/Inv_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"

UInv_InventoryComponent::UInv_InventoryComponent() : InventoryArray(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, InventoryArray);
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult Result = InventoryMenu->GetGridAvailability(ItemComponent);
	if (!Result.HasAvailability())
	{
		OnItemAddingFailed.Broadcast();
		return;
	}
	
	Result.Item = InventoryArray.FindItemByTag(ItemComponent->GetItemSpec().GetItemTag());
	if (Result.bIsStackable && Result.Item.IsValid())
	{
		Server_AddStacksToItem(ItemComponent, Result.StackCountToAdd);
		OnStackChanged.Broadcast(Result);
	}
	else
	{
		Server_AddNewItem(ItemComponent, Result.StackCountToAdd);
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount)
{
	UInv_InventoryItem* Item = InventoryArray.AddItem(ItemComponent);
	Item->SetStackCount(StackCount);
	
	const ENetMode NetMode = GetOwner()->GetNetMode();
	if (NetMode == NM_ListenServer || NetMode == NM_Standalone)
	{
		OnItemAdded.Broadcast(Item);
	}
	
	// TODO: Tell Item Component to destroy it's owner
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount)
{
	const FGameplayTag& ItemTag = ItemComponent->GetItemSpec().GetItemTag();
	UInv_InventoryItem* Item = InventoryArray.FindItemByTag(ItemTag);
	if (!ensure(IsValid(Item))) return;
	
	Item->AddStackCount(StackCount);
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

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
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