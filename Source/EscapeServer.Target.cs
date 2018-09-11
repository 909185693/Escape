// Copyright 2018 by January. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EscapeServerTarget : TargetRules
{
	public EscapeServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

        bUsesSteam = true;

        ExtraModuleNames.AddRange( new string[] { "Escape" } );
	}
}
