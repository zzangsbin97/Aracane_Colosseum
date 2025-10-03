// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Aracane_Colosseum : ModuleRules
{
	public Aracane_Colosseum(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
