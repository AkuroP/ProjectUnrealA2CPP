// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class A2CPP : ModuleRules
{
	public A2CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
