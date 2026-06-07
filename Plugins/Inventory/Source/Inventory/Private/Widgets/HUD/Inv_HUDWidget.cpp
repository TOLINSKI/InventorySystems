// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/HUD/Inv_HUDWidget.h"

#include "InventoryManagement/Inv_InventoryComponent.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Widgets/HUD/Inv_InfoMessageWidget.h"

void UInv_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer()))
	{
		InfoMessageWidget->SetText(FText::FromString("Can't fit in inventory"));
		InventoryComponent->OnItemAddingFailed.AddDynamic(InfoMessageWidget, &UInv_InfoMessageWidget::ShowMessage);
	}
}

