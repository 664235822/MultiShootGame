// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiShootGame : ModuleRules
{
	public MultiShootGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "AIModule", "AnimGraphRuntime", "PhysicsCore", "GameplayCameras", "GameplayTasks" 
		});
	}
}