#include "CoreMinimal.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Core/fp.hpp"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace {

using FForbiddenSourcePattern =
    ForbocAI::Game::Data::Automation::Rtk::Compliance::FPattern;
using FRTKComplianceSettings =
    ForbocAI::Game::Data::Automation::Rtk::Compliance::FSettings;

/** User Story: As a tests consumer, I need to invoke rtkcompliance settings through a stable signature so the tests workflow remains explicit and composable. @fn const FRTKComplianceSettings &RTKComplianceSettings() */
const FRTKComplianceSettings &RTKComplianceSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.RtkCompliance;
}

/** User Story: As a tests consumer, I need to invoke is header or source file through a stable signature so the tests workflow remains explicit and composable. @fn bool IsHeaderOrSourceFile(const FString &Path) */
bool IsHeaderOrSourceFile(const FString &Path) {
  return RTKComplianceSettings().Source.SourceFileSuffixes.ContainsByPredicate(
      [&Path](const FString &Suffix) {
        return Path.EndsWith(Suffix);
      });
}

/** User Story: As a tests consumer, I need to invoke normalized path through a stable signature so the tests workflow remains explicit and composable. @fn FString NormalizedPath(FString Path) */
FString NormalizedPath(FString Path) {
  FPaths::NormalizeFilename(Path);
  return Path;
}

/** User Story: As a tests consumer, I need to invoke is allowed store boundary through a stable signature so the tests workflow remains explicit and composable. @fn bool IsAllowedStoreBoundary(const FString &Path) */
bool IsAllowedStoreBoundary(const FString &Path) {
  const FString Normal = NormalizedPath(Path);
  return RTKComplianceSettings().Source.AllowedBoundaryFragments.ContainsByPredicate(
      [&Normal](const FString &Fragment) {
        return Normal.Contains(Fragment) || Normal.EndsWith(Fragment);
      });
}

/** User Story: As a tests consumer, I need to invoke should audit file through a stable signature so the tests workflow remains explicit and composable. @fn bool ShouldAuditFile(const FString &Path) */
bool ShouldAuditFile(const FString &Path) {
  return IsHeaderOrSourceFile(Path) && !IsAllowedStoreBoundary(Path);
}

/** User Story: As a tests consumer, I need to invoke source files through a stable signature so the tests workflow remains explicit and composable. @fn TArray<FString> SourceFiles() */
TArray<FString> SourceFiles() {
  TArray<FString> Files;
  const FRTKComplianceSettings &Settings = RTKComplianceSettings();
  const FString SourceDir =
      FPaths::Combine(FPaths::ProjectDir(), Settings.Source.SourceDirectoryName);
  IFileManager::Get().FindFilesRecursive(
      Files, *SourceDir, *Settings.Source.SourceSearchPattern, true, false);
  return func::filter_array<FString>(Files, ShouldAuditFile);
}

/** User Story: As a tests consumer, I need to invoke load source content through a stable signature so the tests workflow remains explicit and composable. @fn func::Maybe<FString> LoadSourceContent(const FString &Path) */
func::Maybe<FString> LoadSourceContent(const FString &Path) {
  FString Content;
  return func::from_nullable_value(
      Content, FFileHelper::LoadFileToString(Content, *Path));
}

/** User Story: As a tests consumer, I need to invoke compliance message through a stable signature so the tests workflow remains explicit and composable. @fn FString ComplianceMessage(const FString &Format, const TArray<FStringFormatArg> &Args) */
FString ComplianceMessage(const FString &Format,
                          const TArray<FStringFormatArg> &Args) {
  return FString::Format(*Format, Args);
}

/** User Story: As a tests consumer, I need to invoke count pattern violation through a stable signature so the tests workflow remains explicit and composable. @fn int32 CountPatternViolation(FAutomationTestBase &Test, const FString &Path, const FString &Content, const FForbiddenSourcePattern &Pattern) */
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

/** User Story: As a tests consumer, I need to invoke count forbidden source patterns in file through a stable signature so the tests workflow remains explicit and composable. @fn int32 CountForbiddenSourcePatternsInFile( FAutomationTestBase &Test, const TArray<FForbiddenSourcePattern> &Patterns, const FString &Path) */
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

/** User Story: As a tests consumer, I need to invoke count forbidden source patterns through a stable signature so the tests workflow remains explicit and composable. @fn int32 CountForbiddenSourcePatterns(FAutomationTestBase &Test) */
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

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FRTKComplianceStoreBoundary::RunTest(const FString &Parameters) */
bool FRTKComplianceStoreBoundary::RunTest(const FString &Parameters) {
  (void)Parameters;
  TestEqual(RTKComplianceSettings().StoreBoundary.StoreBoundaryLabel,
            CountForbiddenSourcePatterns(*this),
            RTKComplianceSettings().Violation.CleanViolationCount);
  return true;
}
