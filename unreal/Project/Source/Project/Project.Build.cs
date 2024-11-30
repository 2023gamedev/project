// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project : ModuleRules
{
	public Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "AIModule", "GameplayTasks", "NavigationSystem", "ProceduralMeshComponent", "Niagara"});

        PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Sockets", "Networking" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateIncludePaths.AddRange(new string[]
        {
            "Project/Protocol/"
        });

        // Windows 플랫폼에 대해서만 예외 처리기 활성화
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // 예외 처리 활성화 (C++ 예외 처리기 활성화)
            PublicDefinitions.Add("UE_BUILD_EXCEPTIONS=1");
        }
    }
}
