// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/HUD/Inv_InfoMessageWidget.h"
#include "Components/TextBlock.h"

void UInv_InfoMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BP_HideMessage();
}

bool UInv_InfoMessageWidget::IsActive() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(MessageTimer);
}

void UInv_InfoMessageWidget::SetText(const FText& Message)
{
	Text_Message->SetText(Message);
}

void UInv_InfoMessageWidget::ShowMessage()
{
	GetWorld()->GetTimerManager().SetTimer(MessageTimer, this, &ThisClass::BP_HideMessage, MessageLifetime);
	BP_ShowMessage();
}

