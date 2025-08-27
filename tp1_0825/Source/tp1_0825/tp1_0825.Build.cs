// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class tp1_0825 : ModuleRules
{
	public tp1_0825(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
