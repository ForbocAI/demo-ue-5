
using UnrealBuildTool;
using System;
using System.Collections.Generic;

public class DemoProjectEditorTarget : TargetRules
{
	public DemoProjectEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("DemoProject");
		EnablePlugins.Add("ForbocAI_SDK");
	}
}
