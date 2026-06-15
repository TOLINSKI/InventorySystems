// Copyright Chaos Box Games 2026, All rights reserved.


#include "InventoryManagement/Inv_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_StackFragment.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/Inv_InventoryWidgetBase.h"

UInv_InventoryComponent::UInv_InventoryComponent() : InventoryArray(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bHideMouseCursorOnClosed = true;
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
		Server_AddStacksToItem(ItemComponent, Result.StackCountToAdd, Result.Remainder);
		OnStackChanged.Broadcast(Result);
	}
	else
	{
		Server_AddNewItem(ItemComponent, Result.StackCountToAdd, Result.bIsStackable ? Result.Remainder : 0);
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	UInv_InventoryItem* Item = InventoryArray.AddItem(ItemComponent);
	Item->SetStackCount(StackCount);
	
	const ENetMode NetMode = GetOwner()->GetNetMode();
	if (NetMode == NM_ListenServer || NetMode == NM_Standalone)
	{
		OnItemAdded.Broadcast(Item);
	}
	
	if (Remainder == 0)
	{
		ItemComponent->PickUp();
	}
	else 
	{
		FInv_StackFragment* StackFragment = ItemComponent->GetMutableItemSpec().GetMutableFragment<FInv_StackFragment>();
		StackFragment->SetStackCount(Remainder);
		ItemComponent->PrintStackCount();
	}
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	const FGameplayTag& ItemTag = ItemComponent->GetItemSpec().GetItemTag();
	UInv_InventoryItem* Item = InventoryArray.FindItemByTag(ItemTag);
	if (!ensure(IsValid(Item))) return;
	
	Item->AddStackCount(StackCount);
	
	if (Remainder == 0)
	{
		ItemComponent->PickUp();
	}
	else 
	{
		FInv_StackFragment* StackFragment = ItemComponent->GetMutableItemSpec().GetMutableFragment<FInv_StackFragment>();
		StackFragment->SetStackCount(Remainder);
		ItemComponent->PrintStackCount();
	}
}

void UInv_InventoryComponent::Server_DropItem_Implementation(UInv_InventoryItem* Item, int32 AmountToRemove, bool bSpawnDroppedItem)
{
	const int32 StackCount = Item->GetStackCount();
	Item->SetStackCount(FMath::Max(StackCount - AmountToRemove, 0));
	if (Item->GetStackCount() == 0)
	{
		InventoryArray.RemoveItem(Item);
	}
	if (bSpawnDroppedItem)
	{
		SpawnDroppedItem(Item, AmountToRemove);
	}
}

void UInv_InventoryComponent::SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount) const
{
	const FVector PawnLocation = OwningPlayer->GetPawn()->GetActorLocation();
	const FVector Direction = OwningPlayer->GetPawn()->GetActorForwardVector();
	const FVector SpawnOrigin = PawnLocation + 300.f * Direction;
	
	FVector RandomSpawnLocation = SpawnOrigin + FMath::VRand() * FMath::RandRange(-200.f, 200.f);
	RandomSpawnLocation.Z = PawnLocation.Z - 90.f;
	
	Item->GetItemSpec().SpawnItem(this, RandomSpawnLocation, FRotator::ZeroRotator, StackCount);
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

	OwningPlayer = Cast<APlayerController>(GetOwner());
	checkf(OwningPlayer.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner."))
	if (!OwningPlayer->IsLocalController()) return;

	ConstructInventory();
	CloseInventoryMenu();
}

void UInv_InventoryComponent::ConstructInventory()
{
	InventoryMenu = CreateWidget<UInv_InventoryWidgetBase>(OwningPlayer.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
}