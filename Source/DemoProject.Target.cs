
using UnrealBuildTool;
using System;
using System.Collections.Generic;

public class DemoProjectTarget : TargetRules
{
	public DemoProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
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
