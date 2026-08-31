#include "Features/Systems/State/StateSlice.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Systems/Bots/AI/AISlice.h"
#include "Features/Systems/Bots/Behavior/BehaviorSlice.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"
#include "Features/Systems/Bots/Horses/HorsesSlice.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessSlice.h"
#include "Features/Systems/Bots/Pipeline/PipelineSlice.h"
#include "Features/Systems/Bots/Position/PositionSlice.h"
#include "Features/Systems/Bots/Stats/StatsSlice.h"
#include "Features/Systems/Bots/BotsSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/ForbocAI/Protocol/ProtocolSlice.h"
#include "Features/Systems/Initialization/InitializationAdapters.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Landmarks/LandmarksSlice.h"
#include "Features/Systems/Level/SystemsLevelSlice.h"
#include "Features/Systems/Lifecycle/LifecycleSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Observed/ObservedSlice.h"
#include "Features/Systems/Projection/ProjectionSlice.h"
#include "Features/Systems/Reduction/ReductionSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/Spawn/SystemsSpawnSlice.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/SystemsThunks.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/UI/UISlice.h"
#include "HAL/PlatformTime.h"

#include <functional>

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSlice {
namespace {

using FRootReducerMap = rtk::ReducersMapObject<FRuntimeState>;
using FRootReducerBinding = std::function<FRootReducerMap(FRootReducerMap)>;

/** User Story: As a features systems state consumer, I need to invoke runtime milliseconds through a stable signature so the features systems state workflow remains explicit and composable. @fn double RuntimeMilliseconds(double ElapsedSeconds, const FRuntimeState &State) */
double RuntimeMilliseconds(double ElapsedSeconds,
                           const FRuntimeState &State) {
  return ElapsedSeconds *
         State.UI.Settings.StatsOverlay.Measurement.Diagnostics.SecondsToMilliseconds;
}

/**
 * @fn template <typename SliceState> FRootReducerBinding RootReducerBinding(SliceState FRuntimeState::*Member, const rtk::CaseReducer<SliceState> &Reducer)
 * @brief Declares one root-state member and its owning slice reducer.
 * @param Member Root-state member owned by the slice.
 * @param Reducer Slice reducer registered for that member.
 * @return Unary catalog binding consumed by the root reducer fold.
 *
 * User Story: As a runtime-store maintainer, I need slice membership declared as
 * data so adding a domain extends one catalog instead of another reducer chain.
 */
template <typename SliceState>
FRootReducerBinding
RootReducerBinding(SliceState FRuntimeState::*Member,
                   const rtk::CaseReducer<SliceState> &Reducer) {
  return [Member, Reducer](FRootReducerMap Acc) {
    Acc.reducer(Member, Reducer);
    return Acc;
  };
}

/**
 * @fn const TArray<FRootReducerBinding> &RootReducerBindings()
 * @brief Returns the declaration catalog for every durable runtime slice.
 * @return Stable root reducer bindings in deterministic reduction order.
 *
 * User Story: As a game developer, I need every durable feature composed into
 * one root store through a reviewable domain catalog.
 */
const TArray<FRootReducerBinding> &RootReducerBindings() {
  static const TArray<FRootReducerBinding> Bindings = {
      RootReducerBinding(&FRuntimeState::Player,
                         PlayerSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Systems,
                         SystemsSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Level,
                         LevelSystemSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Rendering,
                         RenderingSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Lifecycle,
                         RuntimeLifecycleSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::ForbocAIProtocol,
                         ForbocAIProtocolSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Dialogue,
                         DialogueSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Interaction,
                         InteractionSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Speech,
                         SpeechSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::UI, UISlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Terrain,
                         TerrainSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Spawn,
                         SpawnSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Landmarks,
                         LandmarkSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Townspeople,
                         TownspersonSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Horses,
                         HorseSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Nature,
                         NatureSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::Bots, BotSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotCore,
                         BotCoreSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotStats,
                         BotStatsSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotPosition,
                         BotPositionSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotAI,
                         BotAISlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotGoals,
                         BotGoalSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotOrchestrator,
                         BotOrchestratorSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotOrchestratorReadiness,
                         BotOrchestratorReadinessSlice::GetSlice().Reducer),
      RootReducerBinding(&FRuntimeState::BotPipeline,
                         BotPipelineSlice::GetSlice().Reducer)};
  return Bindings;
}

/**
 * @fn FRootReducerMap RootReducerMap()
 * @brief Folds the root reducer catalog into the RTK reducer map.
 * @return Composed root reducer map consumed by combineReducers.
 *
 * User Story: As a runtime-store maintainer, I need one reusable composer to
 * assemble all feature reducers without hand-wired chaining.
 */
FRootReducerMap RootReducerMap() {
  return func::fold_array<FRootReducerBinding, FRootReducerMap>(
      RootReducerBindings(), FRootReducerMap(),
      [](const FRootReducerMap &Acc, const FRootReducerBinding &Binding) {
        return Binding(Acc);
      });
}

/**
 * @fn rtk::CaseReducer<FRuntimeState> WithRootDiagnostics( const rtk::CaseReducer<FRuntimeState> &CombinedReducers)
 * @brief Decorates the composed reducer with runtime and ECS measurements.
 * @param CombinedReducers Pure combined feature reducer.
 * @return Root reducer that records reduction and projection diagnostics.
 *
 * User Story: As a runtime diagnostician, I need one measured reducer boundary
 * so slice composition and ECS projection costs remain distinguishable.
 */
rtk::CaseReducer<FRuntimeState> WithRootDiagnostics(
    const rtk::CaseReducer<FRuntimeState> &CombinedReducers) {
  return [CombinedReducers](const FRuntimeState &State,
                            const rtk::AnyAction &Action) {
    const double RootStartedSeconds = FPlatformTime::Seconds();
    const double CombinedStartedSeconds = FPlatformTime::Seconds();
    FRuntimeState Combined = CombinedReducers(State, Action);
    const double CombinedFinishedSeconds = FPlatformTime::Seconds();
    FRuntimeState RuntimeReduced =
        RuntimeReducers::ReduceRuntimeAction(Combined, Action);
    const double ProjectionStartedSeconds = FPlatformTime::Seconds();
    FRuntimeState Projected =
        RuntimeReducers::ShouldProjectRuntimeAction(Action)
            ? RuntimeReducers::ReduceRuntimeProjected(RuntimeReduced)
            : RuntimeReduced;
    const double ProjectionFinishedSeconds = FPlatformTime::Seconds();
    const double RootFinishedSeconds = FPlatformTime::Seconds();
    const ecs::FWorldInspection Inspection =
        ecs::inspectWorld(Projected.Ecs.World);
    Projected.ReducerDiagnostics = {
        Action.Type,
        RuntimeMilliseconds(CombinedFinishedSeconds - CombinedStartedSeconds,
                            Projected),
        RuntimeMilliseconds(ProjectionFinishedSeconds - ProjectionStartedSeconds,
                            Projected),
        RuntimeMilliseconds(RootFinishedSeconds - RootStartedSeconds, Projected),
        Inspection.Storage.EntityCount,
        Inspection.Storage.ComponentTypeCount};
    return Projected;
  };
}

/** User Story: As a features systems state consumer, I need to invoke root reducer through a stable signature so the features systems state workflow remains explicit and composable. @fn const rtk::CaseReducer<FRuntimeState> &RootReducer() */
const rtk::CaseReducer<FRuntimeState> &RootReducer() {
  static const rtk::CaseReducer<FRuntimeState> Reducer =
      WithRootDiagnostics(rtk::combineReducers(RootReducerMap()));
  return Reducer;
}

} // namespace

/** User Story: As a features systems state consumer, I need to invoke get slice through a stable signature so the features systems state workflow remains explicit and composable. @fn const rtk::Slice<FRuntimeState> &GetSlice() */
const rtk::Slice<FRuntimeState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FRuntimeState>> Slice =
      func::lazy([]() -> rtk::Slice<FRuntimeState> {
        // RTK names are reducer/action metadata, not JSON-authored data.
        return rtk::createSlice<FRuntimeState>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV3BEFECFE806D), RuntimeFactories::CreateInitialState(),
            RootReducer());
      });
  return func::eval(Slice);
}

/** User Story: As a features systems state consumer, I need to invoke request player spawn through a stable signature so the features systems state workflow remains explicit and composable. @fn rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn() */
rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return RuntimeThunks::RequestPlayerSpawn();
}

/** User Story: As a features systems state consumer, I need to invoke request level view payload through a stable signature so the features systems state workflow remains explicit and composable. @fn rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> RequestLevelViewPayload() */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload() {
  return RuntimeThunks::RequestLevelViewPayload();
}

} // namespace RuntimeSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
