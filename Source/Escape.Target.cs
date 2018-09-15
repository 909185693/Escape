// Copyright 2018 by January. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EscapeTarget : TargetRules
{
	public EscapeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

        //bUsesSteam = true;

        ExtraModuleNames.AddRange( new string[] { "Escape" } );
	}
}
