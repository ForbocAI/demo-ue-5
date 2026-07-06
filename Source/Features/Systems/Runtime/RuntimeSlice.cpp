#include "Features/Systems/Runtime/RuntimeSlice.h"

#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Systems/Bots/AI/BotAISlice.h"
#include "Features/Systems/Bots/BotSlice.h"
#include "Features/Systems/Bots/Core/BotCoreSlice.h"
#include "Features/Systems/Bots/Goals/BotGoalSlice.h"
#include "Features/Systems/Bots/Horses/HorseSlice.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesSlice.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineSlice.h"
#include "Features/Systems/Bots/Position/BotPositionSlice.h"
#include "Features/Systems/Bots/Stats/BotStatsSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSlice.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Level/LevelSlice.h"
#include "Features/Systems/Landmarks/LandmarkSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Runtime/RuntimeFactories.h"
#include "Features/Systems/Runtime/RuntimeReducers.h"
#include "Features/Systems/Runtime/RuntimeThunks.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/UI/UISlice.h"
#include "HAL/PlatformTime.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSlice {
namespace {

double RuntimeMilliseconds(double StartedSeconds, double FinishedSeconds,
                           const FRuntimeState &State) {
  return (FinishedSeconds - StartedSeconds) *
         State.UI.RuntimeSettings.StatsOverlay.SecondsToMilliseconds;
}

FRuntimeState ReduceRootWithDiagnostics(
    const rtk::CaseReducer<FRuntimeState> &CombinedReducers,
    const FRuntimeState &State, const rtk::AnyAction &Action) {
  const double RootStartedSeconds = FPlatformTime::Seconds();
  const double CombinedStartedSeconds = FPlatformTime::Seconds();
  FRuntimeState Combined = CombinedReducers(State, Action);
  const double CombinedFinishedSeconds = FPlatformTime::Seconds();
  FRuntimeState RuntimeReduced =
      RuntimeReducers::ReduceRuntimeAction(Combined, Action);
  const double ProjectionStartedSeconds = FPlatformTime::Seconds();
  FRuntimeState Projected =
      RuntimeReducers::ReduceRuntimeProjected(RuntimeReduced);
  const double ProjectionFinishedSeconds = FPlatformTime::Seconds();
  const double RootFinishedSeconds = FPlatformTime::Seconds();
  const ecs::FWorldInspection Inspection =
      ecs::inspectWorld(Projected.Ecs.World);
  Projected.ReducerDiagnostics = {
      Action.Type,
      RuntimeMilliseconds(CombinedStartedSeconds, CombinedFinishedSeconds,
                          Projected),
      RuntimeMilliseconds(ProjectionStartedSeconds, ProjectionFinishedSeconds,
                          Projected),
      RuntimeMilliseconds(RootStartedSeconds, RootFinishedSeconds, Projected),
      Inspection.EntityCount,
      Inspection.ComponentTypeCount};
  return Projected;
}

const rtk::CaseReducer<FRuntimeState> &RootReducer() {
  static const rtk::CaseReducer<FRuntimeState> Reducer = []() {
    rtk::ReducersMapObject<FRuntimeState> Reducers;
    Reducers.reducer(&FRuntimeState::Player, PlayerSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Systems, SystemsSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Level, LevelSystemSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Rendering,
                 RenderingSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Dialogue,
                 DialogueSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Interaction,
                 InteractionSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Speech, SpeechSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::UI, UISlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Terrain, TerrainSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Spawn, SpawnSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Landmarks, LandmarkSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Townspeople,
                 TownspersonSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Horses, HorseSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Nature, NatureSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Bots, BotSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotCore, BotCoreSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotStats, BotStatsSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotPosition,
                 BotPositionSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotAI, BotAISlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotGoals, BotGoalSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotOrchestrator,
                 BotOrchestratorSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotOrchestratorFactories,
                 BotOrchestratorFactoriesSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotPipeline,
                 BotPipelineSlice::GetSlice().Reducer);
    const rtk::CaseReducer<FRuntimeState> CombinedReducers =
        rtk::combineReducers(Reducers);
    return [CombinedReducers](const FRuntimeState &State,
                              const rtk::AnyAction &Action) -> FRuntimeState {
      return ReduceRootWithDiagnostics(CombinedReducers, State, Action);
    };
  }();
  return Reducer;
}

} // namespace

const rtk::Slice<FRuntimeState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FRuntimeState>> Slice =
      func::lazy([]() -> rtk::Slice<FRuntimeState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FRuntimeState>(
            TEXT("runtime"), RuntimeFactories::CreateInitialState(),
            RootReducer());
      });
  return func::eval(Slice);
}

rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return RuntimeThunks::RequestPlayerSpawn();
}

rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload() {
  return RuntimeThunks::RequestLevelViewPayload();
}

} // namespace RuntimeSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
