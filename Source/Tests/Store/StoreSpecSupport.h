#pragma once

#include "CoreMinimal.h"
#include "Core/ecs.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Characters/Bots/CharactersBotsAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"
#include "Features/Entities/Environments/Landmarks/EnvironmentsLandmarksAdapters.h"
#include "Features/Systems/Level/SystemsLevelAdapters.h"
#include "Features/Systems/Bots/Horses/HorsesSlice.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSlice.h"
#include "Features/Systems/Nature/SystemsNatureSlice.h"
#include "Features/Systems/Rendering/Distance/Lod/LodSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/Initialization/InitializationAdapters.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Features/Systems/State/StateSlice.h"
#include "Features/Systems/Spawn/SystemsSpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Misc/AutomationTest.h"
#include "Misc/OutputDevice.h"
#include "Store.h"

using namespace ForbocAI::Game::Level;

namespace {

using ForbocAI::Game::Data::FSettings;
using FStoreSettings =
    ForbocAI::Game::Data::Automation::Store::FSettings;

/**
 * @brief Request object for recursively searching captured redux log lines.
 */
struct FReduxLoggerLineSearchRequest {
  const TArray<FString> *Lines = nullptr;
  FString Needle;
  int32 Index = int32{};
};

/**
 * @brief Captures the runtime redux logger category during automation tests.
 */
class FReduxLoggerCaptureDevice final : public FOutputDevice {
public:
  /**
   * @fn explicit FReduxLoggerCaptureDevice(TArray<FString> &InLines, const FString &InCategory)
   * @brief Stores the caller-owned line buffer used by Serialize.
   * @param InLines Mutable output collection for matching redux log lines.
   *
   * User Story: As a test author, I need a focused UE log sink so automation
   * can prove redux-logger writes through the same output path as the console.
   */
  explicit FReduxLoggerCaptureDevice(TArray<FString> &InLines,
                                     const FString &InCategory)
      : Lines(InLines), Category(*InCategory) {}

  /**
   * @fn void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity, const FName &LineCategory) override
   * @brief Records redux logger messages emitted through UE logging.
   * @param V The log message text.
   * @param Verbosity UE log verbosity for the emitted line.
   * @param LineCategory UE log category that produced the line.
   *
   * User Story: As automation coverage, I need to observe UE console output
   * without changing the runtime store or SDK logger behavior.
   */
  void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity,
                 const FName &LineCategory) override {
    (void)Verbosity;
    CaptureLine(V, LineCategory);
  }

  /** User Story: As a tests store consumer, I need to invoke serialize through a stable signature so the tests store workflow remains explicit and composable. @fn void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity, const FName &LineCategory, double Time) override */
  void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity,
                 const FName &LineCategory, double Time) override {
    (void)Verbosity;
    (void)Time;
    CaptureLine(V, LineCategory);
  }

  /** User Story: As a tests store consumer, I need to invoke serialize record through a stable signature so the tests store workflow remains explicit and composable. @fn void SerializeRecord(const UE::FLogRecord &Record) override */
  void SerializeRecord(const UE::FLogRecord &Record) override {
    FOutputDevice::SerializeRecord(Record);
  }

private:
  /** User Story: As a tests store consumer, I need to invoke capture line through a stable signature so the tests store workflow remains explicit and composable. @fn void CaptureLine(const TCHAR *V, const FName &LineCategory) */
  void CaptureLine(const TCHAR *V, const FName &LineCategory) {
    LineCategory == Category
        ? (Lines.Add(FString(V)), void())
        : void();
  }

  TArray<FString> &Lines;
  FName Category;
};

struct FLabelCursor {
  const TArray<FString> *Labels = nullptr;
  int32 Index = int32();

  /** User Story: As a tests store consumer, I need to invoke next through a stable signature so the tests store workflow remains explicit and composable. @fn FString Next() */
  FString Next() {
    check(Labels != nullptr);
    check(Labels->IsValidIndex(Index));
    const FString Label = (*Labels)[Index];
    ++Index;
    return Label;
  }
};

/** User Story: As a tests store consumer, I need to invoke store labels through a stable signature so the tests store workflow remains explicit and composable. @fn FLabelCursor StoreLabels(const TArray<FString> &Labels) */
FLabelCursor
StoreLabels(const TArray<FString> &Labels) {
  return {&Labels, int32()};
}

/** User Story: As a tests store consumer, I need to invoke required first landmark through a stable signature so the tests store workflow remains explicit and composable. @fn const FLandmark &RequiredFirstLandmark(const TArray<FLandmark> &Landmarks) */
const FLandmark &RequiredFirstLandmark(const TArray<FLandmark> &Landmarks) {
  check(!Landmarks.IsEmpty());
  return Landmarks[Landmarks.Num() - Landmarks.Num()];
}

/** User Story: As a tests store consumer, I need to invoke required townsperson with intent through a stable signature so the tests store workflow remains explicit and composable. @fn const FTownspersonSeed &RequiredTownspersonWithIntent( const TArray<FTownspersonSeed> &Townspeople, ETownspersonInteractionIntent Intent) */
const FTownspersonSeed &RequiredTownspersonWithIntent(
    const TArray<FTownspersonSeed> &Townspeople,
    ETownspersonInteractionIntent Intent) {
  const FTownspersonSeed *Townsperson =
      Townspeople.FindByPredicate(
          [Intent](const FTownspersonSeed &Candidate) {
            return Candidate.InteractionIntent == Intent;
          });
  check(Townsperson);
  return *Townsperson;
}

/** User Story: As a tests store consumer, I need to invoke required horse by mounted rider through a stable signature so the tests store workflow remains explicit and composable. @fn const FHorseRouteSeed &RequiredHorseByMountedRider( const TArray<FHorseRouteSeed> &Horses, bool bMountedRider) */
const FHorseRouteSeed &RequiredHorseByMountedRider(
    const TArray<FHorseRouteSeed> &Horses, bool bMountedRider) {
  const FHorseRouteSeed *Horse =
      Horses.FindByPredicate([bMountedRider](const FHorseRouteSeed &Candidate) {
        return Candidate.bMountedRider == bMountedRider;
      });
  check(Horse);
  return *Horse;
}

/** User Story: As a tests store consumer, I need to invoke domain segments through a stable signature so the tests store workflow remains explicit and composable. @fn TArray<FString> DomainSegments(const FString &Path) */
TArray<FString> DomainSegments(const FString &Path) {
  TArray<FString> Segments;
  const FString Separator = FString::Chr(TCHAR('/'));
  Path.ParseIntoArray(Segments, *Separator, true);
  return Segments;
}

/** User Story: As a tests store consumer, I need to invoke domain key from path through a stable signature so the tests store workflow remains explicit and composable. @fn ecs::DomainPathKey DomainKeyFromPath(const FString &Path) */
ecs::DomainPathKey DomainKeyFromPath(const FString &Path) {
  return ecs::createDomainPathKey(ecs::createDomainPath(DomainSegments(Path)));
}

/** User Story: As a tests store consumer, I need to invoke redux logger action title needle through a stable signature so the tests store workflow remains explicit and composable. @fn FString ReduxLoggerActionTitleNeedle(const FString &Prefix, const rtk::AnyAction &Action) */
FString ReduxLoggerActionTitleNeedle(const FString &Prefix,
                                     const rtk::AnyAction &Action) {
  return Prefix + FString::Chr(TCHAR(' ')) + Action.Type;
}

/** User Story: As a tests store consumer, I need to invoke test authored domain registry through a stable signature so the tests store workflow remains explicit and composable. @fn void TestAuthoredDomainRegistry(FAutomationTestBase *Test, const FString &Label, const ecs::FGraph &Registry, const FSettings &Settings) */
void TestAuthoredDomainRegistry(FAutomationTestBase *Test,
                                const FString &Label,
                                const ecs::FGraph &Registry,
                                const FSettings &Settings) {
  func::for_each_indexed(
      Settings.Ecs.DomainRegistry,
      static_cast<size_t>(Settings.Ecs.DomainRegistry.Num()),
      [Test, &Label, &Registry](
          const ForbocAI::Game::Data::FDomainRegistrationSettings &Domain) {
        Test->TestTrue(Label,
                       Registry.Nodes.Contains(DomainKeyFromPath(Domain.Path)));
      });
}

/** User Story: As a tests store consumer, I need to invoke next redux logger line search request through a stable signature so the tests store workflow remains explicit and composable. @fn FReduxLoggerLineSearchRequest NextReduxLoggerLineSearchRequest(const FReduxLoggerLineSearchRequest &Request) */
FReduxLoggerLineSearchRequest
NextReduxLoggerLineSearchRequest(const FReduxLoggerLineSearchRequest &Request) {
  FReduxLoggerLineSearchRequest Next = Request;
  ++Next.Index;
  return Next;
}

/**
 * @fn bool ContainsReduxLoggerLineRecursive( const FReduxLoggerLineSearchRequest &Request)
 * @brief Recursively checks captured redux logger lines for a substring.
 * @param Request Search payload containing lines, substring, and current index.
 * @return true when the substring appears in any captured line.
 *
 * User Story: As a test maintainer, I need a small pure helper so logger
 * assertions stay separate from the UE output-device effect boundary.
 */
bool ContainsReduxLoggerLineRecursive(
    const FReduxLoggerLineSearchRequest &Request) {
  return Request.Index >= Request.Lines->Num()
             ? false
	             : ((*Request.Lines)[Request.Index].Contains(Request.Needle)
	                    ? true
	                    : ContainsReduxLoggerLineRecursive(
		                          NextReduxLoggerLineSearchRequest(Request)));
}

/**
 * @fn bool ContainsReduxLoggerLine(const FReduxLoggerLineSearchRequest &Request)
 * @brief Checks captured redux logger lines for a substring.
 * @param Request Search payload containing lines and substring.
 * @return true when the substring appears in any captured line.
 *
 * User Story: As automation coverage, I need readable assertions that prove
 * dispatched RTK actions reach the redux-logger UE output sink.
 */
bool ContainsReduxLoggerLine(const FReduxLoggerLineSearchRequest &Request) {
  return ContainsReduxLoggerLineRecursive(
      {Request.Lines, Request.Needle, int32{}});
}

} // namespace
