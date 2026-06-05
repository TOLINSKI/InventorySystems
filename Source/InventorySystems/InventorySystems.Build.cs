// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventorySystems : ModuleRules
{
	public InventorySystems(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"InventorySystems",
			"InventorySystems/Variant_Platforming",
			"InventorySystems/Variant_Platforming/Animation",
			"InventorySystems/Variant_Combat",
			"InventorySystems/Variant_Combat/AI",
			"InventorySystems/Variant_Combat/Animation",
			"InventorySystems/Variant_Combat/Gameplay",
			"InventorySystems/Variant_Combat/Interfaces",
			"InventorySystems/Variant_Combat/UI",
			"InventorySystems/Variant_SideScrolling",
			"InventorySystems/Variant_SideScrolling/AI",
			"InventorySystems/Variant_SideScrolling/Gameplay",
			"InventorySystems/Variant_SideScrolling/Interfaces",
			"InventorySystems/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
