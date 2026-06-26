using System;
using System.IO;
using UnrealBuildTool;

public class DemoProject : ModuleRules
{
	public DemoProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		string ModuleSourceRoot = Path.GetFullPath(Path.Combine(ModuleDirectory, ".."));
		string ProjectRoot = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
		string ForbocSdkPublicRoot = Path.Combine(ProjectRoot, "Plugins", "ForbocAI_SDK", "Source", "ForbocAI_SDK", "Public");
		bool bEnableForbocAISDKDemo = Environment.GetEnvironmentVariable("FORBOC_DEMO_WITH_SDK") == "1";

		PublicDefinitions.Add("WITH_FORBOC_AI_SDK_DEMO=" + (bEnableForbocAISDKDemo ? "1" : "0"));
		PublicIncludePaths.AddRange(new string[] { ModuleDirectory, ModuleSourceRoot, ForbocSdkPublicRoot });
		PrivateIncludePaths.AddRange(new string[] { ModuleDirectory, ModuleSourceRoot, ForbocSdkPublicRoot });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities", "ProceduralMeshComponent", "UMG" });

		if (bEnableForbocAISDKDemo)
		{
			PublicDependencyModuleNames.Add("ForbocAI_SDK");
		}

		PrivateDependencyModuleNames.AddRange(new string[] { "HTTP", "Slate", "SlateCore" });
	}
}
