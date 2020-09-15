// Copyright of Codebird Ltd 2020 - inclusive © 

using UnrealBuildTool;
using System.Collections.Generic;

public class PaxProtoEditorTarget : TargetRules
{
	public PaxProtoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "PaxProto" } );
	}
}
