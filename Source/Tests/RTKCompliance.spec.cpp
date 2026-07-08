#include "CoreMinimal.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Core/ue_fp.hpp"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace {

using FForbiddenSourcePatternSettings =
    ForbocAI::Game::Data::FForbiddenSourcePatternSettings;
using FRTKComplianceAutomationSettings =
    ForbocAI::Game::Data::FRTKComplianceAutomationSettings;

const FRTKComplianceAutomationSettings &RTKComplianceSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.RtkCompliance;
}

bool IsHeaderOrSourceFile(const FString &Path) {
  return RTKComplianceSettings().SourceFileSuffixes.ContainsByPredicate(
      [&Path](const FString &Suffix) {
        return Path.EndsWith(Suffix);
      });
}

FString NormalizedPath(FString Path) {
  FPaths::NormalizeFilename(Path);
  return Path;
}

bool IsAllowedStoreBoundary(const FString &Path) {
  const FString Normal = NormalizedPath(Path);
  return RTKComplianceSettings().AllowedBoundaryFragments.ContainsByPredicate(
      [&Normal](const FString &Fragment) {
        return Normal.Contains(Fragment) || Normal.EndsWith(Fragment);
      });
}

bool ShouldAuditFile(const FString &Path) {
  return IsHeaderOrSourceFile(Path) && !IsAllowedStoreBoundary(Path);
}

TArray<FString> SourceFiles() {
  TArray<FString> Files;
  const FRTKComplianceAutomationSettings &Settings = RTKComplianceSettings();
  const FString SourceDir =
      FPaths::Combine(FPaths::ProjectDir(), Settings.SourceDirectoryName);
  IFileManager::Get().FindFilesRecursive(
      Files, *SourceDir, *Settings.SourceSearchPattern, true, false);
  return func::filter_array<FString>(Files, ShouldAuditFile);
}

func::Maybe<FString> LoadSourceContent(const FString &Path) {
  FString Content;
  return func::from_nullable_value(
      Content, FFileHelper::LoadFileToString(Content, *Path));
}

int32 CountPatternViolation(FAutomationTestBase &Test, const FString &Path,
                            const FString &Content,
                            const FForbiddenSourcePatternSettings &Pattern) {
  const FRTKComplianceAutomationSettings &Settings = RTKComplianceSettings();
  return Content.Contains(Pattern.Token)
             ? (Test.AddError(FString::Printf(*Settings.ViolationMessageFormat,
                                               *Path, *Pattern.Message,
                                               *Pattern.Token)),
                Settings.ViolationCountIncrement)
             : Settings.CleanViolationCount;
}

int32 CountForbiddenSourcePatternsInFile(
    FAutomationTestBase &Test,
    const TArray<FForbiddenSourcePatternSettings> &Patterns,
    const FString &Path) {
  return func::match(
      LoadSourceContent(Path),
      [&Test, &Patterns, &Path](const FString &Content) {
        return func::fold_array<FForbiddenSourcePatternSettings, int32>(
            Patterns, RTKComplianceSettings().CleanViolationCount,
            [&Test, &Path, &Content](
                const int32 &Count,
                const FForbiddenSourcePatternSettings &Pattern) -> int32 {
              return Count + CountPatternViolation(Test, Path, Content, Pattern);
            });
      },
      [&Test, &Path]() -> int32 {
        Test.AddError(FString::Printf(
            *RTKComplianceSettings().SourceReadFailureFormat,
                                      *Path));
        return RTKComplianceSettings().ViolationCountIncrement;
      });
}

int32 CountForbiddenSourcePatterns(FAutomationTestBase &Test) {
  const TArray<FForbiddenSourcePatternSettings> Patterns =
      RTKComplianceSettings().ForbiddenPatterns;
  return func::fold_array<FString, int32>(
      SourceFiles(), RTKComplianceSettings().CleanViolationCount,
      [&Test, &Patterns](const int32 &Count, const FString &Path) -> int32 {
        return Count +
               CountForbiddenSourcePatternsInFile(Test, Patterns, Path);
      });
}

} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FRTKComplianceStoreBoundary,
    "ForbocAI.Runtime.RTKCompliance.StoreBoundary",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRTKComplianceStoreBoundary::RunTest(const FString &Parameters) {
  (void)Parameters;
  TestEqual(RTKComplianceSettings().StoreBoundaryLabel,
            CountForbiddenSourcePatterns(*this),
            RTKComplianceSettings().CleanViolationCount);
  return true;
}
