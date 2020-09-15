// Copyright of Codebird Ltd 2020 - inclusive © 

using UnrealBuildTool;
using System.Collections.Generic;

public class PaxProtoTarget : TargetRules
{
	public PaxProtoTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "PaxProto" } );
	}
}
