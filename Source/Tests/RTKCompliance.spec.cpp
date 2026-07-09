#include "CoreMinimal.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Core/ue_fp.hpp"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace {

using FForbiddenSourcePattern =
    ForbocAI::Game::Data::Automation::Rtk::Compliance::FPattern;
using FRTKComplianceSettings =
    ForbocAI::Game::Data::Automation::Rtk::Compliance::FSettings;

const FRTKComplianceSettings &RTKComplianceSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.RtkCompliance;
}

bool IsHeaderOrSourceFile(const FString &Path) {
  return RTKComplianceSettings().Source.SourceFileSuffixes.ContainsByPredicate(
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
  return RTKComplianceSettings().Source.AllowedBoundaryFragments.ContainsByPredicate(
      [&Normal](const FString &Fragment) {
        return Normal.Contains(Fragment) || Normal.EndsWith(Fragment);
      });
}

bool ShouldAuditFile(const FString &Path) {
  return IsHeaderOrSourceFile(Path) && !IsAllowedStoreBoundary(Path);
}

TArray<FString> SourceFiles() {
  TArray<FString> Files;
  const FRTKComplianceSettings &Settings = RTKComplianceSettings();
  const FString SourceDir =
      FPaths::Combine(FPaths::ProjectDir(), Settings.Source.SourceDirectoryName);
  IFileManager::Get().FindFilesRecursive(
      Files, *SourceDir, *Settings.Source.SourceSearchPattern, true, false);
  return func::filter_array<FString>(Files, ShouldAuditFile);
}

func::Maybe<FString> LoadSourceContent(const FString &Path) {
  FString Content;
  return func::from_nullable_value(
      Content, FFileHelper::LoadFileToString(Content, *Path));
}

FString ComplianceMessage(const FString &Format,
                          const TArray<FStringFormatArg> &Args) {
  return FString::Format(*Format, Args);
}

int32 CountPatternViolation(FAutomationTestBase &Test, const FString &Path,
                            const FString &Content,
                            const FForbiddenSourcePattern &Pattern) {
  const FRTKComplianceSettings &Settings = RTKComplianceSettings();
  return Content.Contains(Pattern.Token)
             ? (Test.AddError(ComplianceMessage(
                    Settings.Violation.ViolationMessageFormat,
                    {FStringFormatArg(Path), FStringFormatArg(Pattern.Message),
                     FStringFormatArg(Pattern.Token)})),
                Settings.Violation.ViolationCountIncrement)
             : Settings.Violation.CleanViolationCount;
}

int32 CountForbiddenSourcePatternsInFile(
    FAutomationTestBase &Test,
    const TArray<FForbiddenSourcePattern> &Patterns,
    const FString &Path) {
  return func::match(
      LoadSourceContent(Path),
      [&Test, &Patterns, &Path](const FString &Content) {
        return func::fold_array<FForbiddenSourcePattern, int32>(
            Patterns, RTKComplianceSettings().Violation.CleanViolationCount,
            [&Test, &Path, &Content](
                const int32 &Count,
                const FForbiddenSourcePattern &Pattern) -> int32 {
              return Count + CountPatternViolation(Test, Path, Content, Pattern);
            });
      },
      [&Test, &Path]() -> int32 {
        Test.AddError(ComplianceMessage(
            RTKComplianceSettings().Violation.SourceReadFailureFormat,
            {FStringFormatArg(Path)}));
        return RTKComplianceSettings().Violation.ViolationCountIncrement;
      });
}

int32 CountForbiddenSourcePatterns(FAutomationTestBase &Test) {
  const TArray<FForbiddenSourcePattern> Patterns =
      RTKComplianceSettings().ForbiddenPatterns;
  return func::fold_array<FString, int32>(
      SourceFiles(), RTKComplianceSettings().Violation.CleanViolationCount,
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
  TestEqual(RTKComplianceSettings().StoreBoundary.StoreBoundaryLabel,
            CountForbiddenSourcePatterns(*this),
            RTKComplianceSettings().Violation.CleanViolationCount);
  return true;
}
