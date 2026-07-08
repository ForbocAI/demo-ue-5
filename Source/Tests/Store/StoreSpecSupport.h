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
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
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
   * @brief Stores the caller-owned line buffer used by Serialize.
   * @signature explicit FReduxLoggerCaptureDevice(TArray<FString> &InLines)
   * @param InLines Mutable output collection for matching redux log lines.
   *
   * User Story: As a test author, I need a focused UE log sink so automation
   * can prove redux-logger writes through the same output path as the console.
   */
  explicit FReduxLoggerCaptureDevice(TArray<FString> &InLines,
                                     const FString &InCategory)
      : Lines(InLines), Category(*InCategory) {}

  /**
   * @brief Records redux logger messages emitted through UE logging.
   * @signature void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity, const FName &LineCategory)
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

  void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity,
                 const FName &LineCategory, double Time) override {
    (void)Verbosity;
    (void)Time;
    CaptureLine(V, LineCategory);
  }

  void SerializeRecord(const UE::FLogRecord &Record) override {
    FOutputDevice::SerializeRecord(Record);
  }

private:
  void CaptureLine(const TCHAR *V, const FName &LineCategory) {
    LineCategory == Category
        ? (Lines.Add(FString(V)), void())
        : void();
  }

  TArray<FString> &Lines;
  FName Category;
};

struct FStoreLabelCursor {
  const TArray<FString> *Labels = nullptr;
  int32 Index = int32();

  FString Next() {
    check(Labels != nullptr);
    check(Labels->IsValidIndex(Index));
    const FString Label = (*Labels)[Index];
    ++Index;
    return Label;
  }
};

FStoreLabelCursor
StoreLabels(const TArray<FString> &Labels) {
  return {&Labels, int32()};
}

const FLandmark &RequiredFirstLandmark(const TArray<FLandmark> &Landmarks) {
  check(!Landmarks.IsEmpty());
  return Landmarks[Landmarks.Num() - Landmarks.Num()];
}

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

const FHorseRouteSeed &RequiredHorseByMountedRider(
    const TArray<FHorseRouteSeed> &Horses, bool bMountedRider) {
  const FHorseRouteSeed *Horse =
      Horses.FindByPredicate([bMountedRider](const FHorseRouteSeed &Candidate) {
        return Candidate.bMountedRider == bMountedRider;
      });
  check(Horse);
  return *Horse;
}

TArray<FString> DomainSegments(const FString &Path) {
  TArray<FString> Segments;
  const FString Separator = FString::Chr(TCHAR('/'));
  Path.ParseIntoArray(Segments, *Separator, true);
  return Segments;
}

ecs::DomainPathKey DomainKeyFromPath(const FString &Path) {
  return ecs::createDomainPathKey(ecs::createDomainPath(DomainSegments(Path)));
}

FString ReduxLoggerActionTitleNeedle(const FString &Prefix,
                                     const rtk::AnyAction &Action) {
  return Prefix + FString::Chr(TCHAR(' ')) + Action.Type;
}

void TestAuthoredDomainRegistry(FAutomationTestBase *Test,
                                const FString &Label,
                                const ecs::FDomainGraph &Registry,
                                const FSettings &Settings) {
  func::for_each_indexed(
      Settings.Ecs.DomainRegistry,
      static_cast<size_t>(Settings.Ecs.DomainRegistry.Num()),
      [Test, &Label, &Registry](
          const ForbocAI::Game::Data::FEcsDomainRegistrationSettings &Domain) {
        Test->TestTrue(Label,
                       Registry.Nodes.Contains(DomainKeyFromPath(Domain.Path)));
      });
}

FReduxLoggerLineSearchRequest
NextReduxLoggerLineSearchRequest(const FReduxLoggerLineSearchRequest &Request) {
  FReduxLoggerLineSearchRequest Next = Request;
  ++Next.Index;
  return Next;
}

/**
 * @brief Recursively checks captured redux logger lines for a substring.
 * @signature bool ContainsReduxLoggerLineRecursive(const FReduxLoggerLineSearchRequest &Request)
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
 * @brief Checks captured redux logger lines for a substring.
 * @signature bool ContainsReduxLoggerLine(const FReduxLoggerLineSearchRequest &Request)
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
