
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

		if (Environment.GetEnvironmentVariable("FORBOC_DEMO_WITH_SDK") == "1")
		{
			EnablePlugins.Add("ForbocAI_SDK");
		}
		else
		{
			DisablePlugins.Add("ForbocAI_SDK");
		}
	}
}
