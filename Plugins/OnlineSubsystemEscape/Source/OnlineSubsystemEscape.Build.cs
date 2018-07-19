// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemEscape : ModuleRules
{
	public OnlineSubsystemEscape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDefinitions.Add("ONLINESUBSYSTEMESCAPE_PACKAGE=1");

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
                "Sockets",
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
