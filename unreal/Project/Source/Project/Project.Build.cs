// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project : ModuleRules
{
	public Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "AIModule", "GameplayTasks", "NavigationSystem"});

        PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

        PrivateIncludePaths.AddRange(new string[]
        {
            "Project/Protocol/"
        });
    }
}
