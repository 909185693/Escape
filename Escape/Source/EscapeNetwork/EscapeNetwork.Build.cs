// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EscapeNetwork : ModuleRules
{
	public EscapeNetwork(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "EscapeNetwork/Public",
                "EscapeNetwork/Classes",
                "../../Public"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
                "EscapeNetwork/Private",
			}
        );

        PublicDependencyModuleNames.AddRange(
			new string[]
			{

			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
            {
                "Core",
                "CoreUObject",
				"Engine",
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
