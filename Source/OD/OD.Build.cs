// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OD : ModuleRules
{
	public OD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"CommonUI",
			"CommonInput",
			"AIModule",
			"NavigationSystem",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayTasks",
			"UMG",
			"Paper2D",
			"Slate",
			"SlateCore",
			"AnimGraphRuntime",
			"PropertyPath",
			"NetCore",
			"Networking",
			"Niagara",
			"GameplayDebugger",
			"OnlineSubsystem",
			"OnlineSubsystemNull",
			"OnlineSubsystemUtils"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ReplicationGraph"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(
			new string[] {
				"OD/Public",
				"OD/Private"
			}
		);
		
	
        

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
