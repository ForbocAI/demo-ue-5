#include "CoreMinimal.h"
#include "Core/ue_fp.hpp"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace {

struct FForbiddenSourcePattern {
  FString Token;
  FString Message;
};

bool IsHeaderOrSourceFile(const FString &Path) {
  return Path.EndsWith(TEXT(".h")) || Path.EndsWith(TEXT(".hpp")) ||
         Path.EndsWith(TEXT(".cpp"));
}

FString NormalizedPath(FString Path) {
  FPaths::NormalizeFilename(Path);
  return Path;
}

bool IsAllowedStoreBoundary(const FString &Path) {
  const FString Normal = NormalizedPath(Path);
  return Normal.Contains(TEXT("/Source/Tests/")) ||
         Normal.EndsWith(TEXT("/Source/Store.cpp")) ||
         Normal.EndsWith(
             TEXT("/Source/Features/Systems/Actions.cpp")) ||
         Normal.EndsWith(
             TEXT("/Source/Features/Systems/Selectors.cpp"));
}

bool ShouldAuditFile(const FString &Path) {
  return IsHeaderOrSourceFile(Path) && !IsAllowedStoreBoundary(Path);
}

TArray<FString> SourceFiles() {
  TArray<FString> Files;
  const FString SourceDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Source"));
  IFileManager::Get().FindFilesRecursive(
      Files, *SourceDir, TEXT("*.*"), true, false);
  return func::filter_array<FString>(Files, ShouldAuditFile);
}

TArray<FForbiddenSourcePattern> ForbiddenPatterns() {
  return {
      {TEXT("#include \"Store.h\""),
       TEXT("Use RuntimeActions/RuntimeSelectors instead of importing Store.h")},
      {TEXT("Store::GetStore("),
       TEXT("Use RuntimeActions/RuntimeSelectors instead of direct store access")},
      {TEXT("Store::GetStore()."),
       TEXT("Use RuntimeActions/RuntimeSelectors instead of direct store access")},
      {TEXT("LastInitialPatrol"),
       TEXT("Patrol view models are derived data and must not live in RTK state")},
      {TEXT("LastPatrolAdvance"),
       TEXT("Patrol view models are derived data and must not live in RTK state")},
      {TEXT("LastViewDefaults"),
       TEXT("Townsperson default overrides are derived data and must not live in RTK state")},
      {TEXT("LastInteractionOverlap"),
       TEXT("Overlap view models are derived data and must not live in RTK state")},
      {TEXT("State.MovementInput"),
       TEXT("Player movement view models are derived data and must not live in RTK state")}};
}

func::Maybe<FString> LoadSourceContent(const FString &Path) {
  FString Content;
  return func::from_nullable_value(
      Content, FFileHelper::LoadFileToString(Content, *Path));
}

int32 CountPatternViolation(FAutomationTestBase &Test, const FString &Path,
                            const FString &Content,
                            const FForbiddenSourcePattern &Pattern) {
  return Content.Contains(Pattern.Token)
             ? (Test.AddError(FString::Printf(TEXT("%s: %s (%s)"), *Path,
                                               *Pattern.Message,
                                               *Pattern.Token)),
                1)
             : 0;
}

int32 CountForbiddenSourcePatternsInFile(
    FAutomationTestBase &Test, const TArray<FForbiddenSourcePattern> &Patterns,
    const FString &Path) {
  return func::match(
      LoadSourceContent(Path),
      [&Test, &Patterns, &Path](const FString &Content) {
        return func::fold_array<FForbiddenSourcePattern, int32>(
            Patterns, 0,
            [&Test, &Path, &Content](
                const int32 &Count,
                const FForbiddenSourcePattern &Pattern) -> int32 {
              return Count + CountPatternViolation(Test, Path, Content, Pattern);
            });
      },
      [&Test, &Path]() -> int32 {
        Test.AddError(FString::Printf(TEXT("Unable to read source file: %s"),
                                      *Path));
        return 1;
      });
}

int32 CountForbiddenSourcePatterns(FAutomationTestBase &Test) {
  const TArray<FForbiddenSourcePattern> Patterns = ForbiddenPatterns();
  return func::fold_array<FString, int32>(
      SourceFiles(), 0,
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
  TestEqual(TEXT("No forbidden RTK store boundary or scratchpad patterns"),
            CountForbiddenSourcePatterns(*this), 0);
  return true;
}
