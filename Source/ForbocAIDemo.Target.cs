
using UnrealBuildTool;
using System;
using System.Collections.Generic;

public class ForbocAIDemoTarget : TargetRules
{
	public ForbocAIDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("ForbocAIDemo");
		EnablePlugins.Add("ForbocAI_SDK");
	}
}
