// Copyright Chaos Box Games 2026, All rights reserved.


#include "Equipement/Inv_EquipmentComponent.h"

#include "Inventory.h"
#include "Equipement/Inv_EquipmentProxyActor.h"
#include "GameFramework/Character.h"
#include "InventoryManagement/Inv_InventoryComponent.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/UsableFragments/Inv_UsableFragment.h"
#include "Kismet/GameplayStatics.h"


UInv_EquipmentComponent::UInv_EquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetOwner()->IsA<APlayerController>() || !GetOwner()->HasAuthority())
	{
		return;	
	}
	
	Setup(Cast<APlayerController>(GetOwner()));
}

void UInv_EquipmentComponent::Setup(APlayerController* PlayerController)
{
	OwningPlayer = PlayerController;
	
	UInv_InventoryComponent* IC = UInv_InventoryStatics::GetInventoryComponent(PlayerController);
	IC->OnItemEquipped.AddDynamic(this, &ThisClass::UInv_EquipmentComponent::OnItemEquipped);
	IC->OnItemUnequipped.AddDynamic(this, &ThisClass::UInv_EquipmentComponent::OnItemUnequipped);
	
	if (AInv_EquipmentProxyActor* Proxy = Cast<AInv_EquipmentProxyActor>(UGameplayStatics::GetActorOfClass(this, AInv_EquipmentProxyActor::StaticClass())))
	{
		ProxyActor = Proxy;
	}
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* Item)
{
	check(IsValid(Item));
	UE_LOG(LogInventory, Warning, TEXT("UInv_EquipmentComponent::OnItemEquipped() +"));

	AttachEquipmentActor(Item, GetOwningCharacterMesh());
	TryAttachEquipmentToProxy(Item);

	// Handle Usable Fragment
	FInv_UsableFragment* UsableFragment = Item->GetMutableItemSpec().GetMutableFragment<FInv_UsableFragment>();
	if (!UsableFragment) return;
	
	UsableFragment->OnUsed(GetOwningPlayer());
}

void UInv_EquipmentComponent::OnItemUnequipped(UInv_InventoryItem* Item)
{
	check(IsValid(Item));
	UE_LOG(LogInventory, Warning, TEXT("UInv_EquipmentComponent::OnItemUnequipped() -"));
	
	DetachEquipmentActor(Item);
	TryDetachEquipmentFromProxy(Item);
	
	// Handle Usable Fragment
	FInv_UsableFragment* UsableFragment = Item->GetMutableItemSpec().GetMutableFragment<FInv_UsableFragment>();
	if (!UsableFragment) return;
	
	UsableFragment->OnUnUsed(GetOwningPlayer());
}

void UInv_EquipmentComponent::AttachEquipmentActor(UInv_InventoryItem* Item, USkeletalMeshComponent* CharacterMesh)
{
	const FInv_ItemSpec& ItemSpec = Item->GetItemSpec();
	const FInv_EquipmentFragment* EquipmentFragment = ItemSpec.GetFragment<FInv_EquipmentFragment>();
	if (!ensure(EquipmentFragment != nullptr)) return;
	
	AActor* NewEquipment = EquippedActors.FindOrAdd(Item->GetItemTag(), GetWorld()->SpawnActor(EquipmentFragment->EquipmentClass)).Get();
	NewEquipment->SetReplicates(true);
	NewEquipment->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipmentFragment->EquipmentSocket);
}

void UInv_EquipmentComponent::TryAttachEquipmentToProxy(UInv_InventoryItem* Item) const
{
	if (AInv_EquipmentProxyActor* Proxy = ProxyActor.Get())
	{
		UInv_EquipmentComponent* ProxyEquipComp = Proxy->FindComponentByClass<UInv_EquipmentComponent>();
		Proxy->FindComponentByClass<UInv_EquipmentComponent>()->AttachEquipmentActor(Item, Proxy->GetMesh());
	}
}

APlayerController* UInv_EquipmentComponent::GetOwningPlayer() const
{
	return Cast<APlayerController>(GetOwner());
}

USkeletalMeshComponent* UInv_EquipmentComponent::GetOwningCharacterMesh() const
{
	return Cast<ACharacter>(GetOwningPlayer()->GetPawnOrSpectator())->GetMesh();
}

void UInv_EquipmentComponent::TryDetachEquipmentFromProxy(UInv_InventoryItem* Item) const
{
	if (AInv_EquipmentProxyActor* Proxy = ProxyActor.Get())
	{
		Proxy->FindComponentByClass<UInv_EquipmentComponent>()->DetachEquipmentActor(Item);
	}
}

void UInv_EquipmentComponent::DetachEquipmentActor(UInv_InventoryItem* Item)
{
	TWeakObjectPtr<AActor> Equipment;
	EquippedActors.RemoveAndCopyValue(Item->GetItemTag(), Equipment);
	Equipment->Destroy();
}

