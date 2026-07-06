using System;
using System.IO;
using UnrealBuildTool;

public class ForbocAIDemo : ModuleRules
{
	public ForbocAIDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		string ModuleSourceRoot = ModuleDirectory;
		string ProjectRoot = Path.GetFullPath(Path.Combine(ModuleDirectory, ".."));
		string ForbocSdkPublicRoot = Path.Combine(ProjectRoot, "Plugins", "ForbocAI_SDK", "Source", "ForbocAI_SDK", "Public");

		PublicDefinitions.Add("WITH_FORBOC_AI_SDK_RUNTIME=1");
		PublicIncludePaths.AddRange(new string[] { ModuleSourceRoot, ForbocSdkPublicRoot });
		PrivateIncludePaths.AddRange(new string[] { ModuleSourceRoot, ForbocSdkPublicRoot });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EnhancedInput", "InputCore", "Json", "JsonUtilities", "ProceduralMeshComponent", "UMG", "ForbocAI_SDK" });

		PrivateDependencyModuleNames.AddRange(new string[] { "HTTP", "Slate", "SlateCore" });
	}
}
