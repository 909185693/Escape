// Copyright 2018 by January. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EscapeClientTarget : TargetRules
{
	public EscapeClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

        bUsesSteam = true;

        ExtraModuleNames.AddRange( new string[] { "Escape" } );
	}
}
