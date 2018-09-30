// Copyright 2018 by January. All Rights Reserved.

using UnrealBuildTool;

public class Escape : ModuleRules
{
	public Escape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "UMG",
            "Core",
            "CoreUObject",
            "Engine",
            "EngineSettings",
            "InputCore",
            "Networking",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "AssetRegistry"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "Crasheye",
            "EscapeNetwork",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
