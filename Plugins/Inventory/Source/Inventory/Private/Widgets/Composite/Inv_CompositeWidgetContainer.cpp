// Copyright Chaos Box Games 2026, All rights reserved.


#include "Widgets/Composite/Inv_CompositeWidgetContainer.h"

#include "Blueprint/WidgetTree.h"

void UInv_CompositeWidgetContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UInv_CompositeWidget* CompositeWidget = Cast<UInv_CompositeWidget>(Widget))
		{
			CompositeWidgets.Add(CompositeWidget);
			CompositeWidget->Collapse();
		}
	});
}

void UInv_CompositeWidgetContainer::ApplyFunction(FuncType Function)
{
	for (auto& Widget : CompositeWidgets)
	{
		Widget->ApplyFunction(Function);
	}
}
