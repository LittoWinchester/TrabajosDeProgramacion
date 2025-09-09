// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP1 : ModuleRules
{
	public TP1(ReadOnlyTargetRules Target) : base(Target)
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
			"TP1",
			"TP1/Variant_Platforming",
			"TP1/Variant_Platforming/Animation",
			"TP1/Variant_Combat",
			"TP1/Variant_Combat/AI",
			"TP1/Variant_Combat/Animation",
			"TP1/Variant_Combat/Gameplay",
			"TP1/Variant_Combat/Interfaces",
			"TP1/Variant_Combat/UI",
			"TP1/Variant_SideScrolling",
			"TP1/Variant_SideScrolling/AI",
			"TP1/Variant_SideScrolling/Gameplay",
			"TP1/Variant_SideScrolling/Interfaces",
			"TP1/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
