
using UnrealBuildTool;
using System;
using System.Collections.Generic;

public class ForbocAIDemoEditorTarget : TargetRules
{
	public ForbocAIDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("ForbocAIDemo");
		EnablePlugins.Add("ForbocAI_SDK");
	}
}
