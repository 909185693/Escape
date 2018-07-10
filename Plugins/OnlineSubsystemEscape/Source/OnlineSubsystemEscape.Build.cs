// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemEscape : ModuleRules
{
	public OnlineSubsystemEscape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"OnlineSubsystemEscape/Public"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"OnlineSubsystemEscape/Private",
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "OnlineSubsystemUtils"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "OnlineSubsystem"
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
			}
			);
	}
}
