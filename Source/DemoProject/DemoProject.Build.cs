using System.IO;
using UnrealBuildTool;

public class DemoProject : ModuleRules
{
	public DemoProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		string ModuleSourceRoot = Path.GetFullPath(Path.Combine(ModuleDirectory, ".."));
		PublicIncludePaths.AddRange(new string[] { ModuleDirectory, ModuleSourceRoot });
		PrivateIncludePaths.AddRange(new string[] { ModuleDirectory, ModuleSourceRoot });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities", "UMG", "ForbocAI_SDK" });

		PrivateDependencyModuleNames.AddRange(new string[] { "HTTP", "Slate", "SlateCore" });
	}
}
