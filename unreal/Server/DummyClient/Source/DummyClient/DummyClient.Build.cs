// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DummyClient : ModuleRules
{
	public DummyClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

		PrivateIncludePaths.AddRange(new string[]
		{
			"DummyClient/Network/"
		});
	}
}
