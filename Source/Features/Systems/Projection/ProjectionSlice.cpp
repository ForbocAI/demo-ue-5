#include "Features/Systems/Projection/ProjectionSlice.h"

#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Systems/Bots/AI/AIActions.h"
#include "Features/Systems/Bots/BotsActions.h"
#include "Features/Systems/Bots/Goals/GoalsActions.h"
#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/Bots/Stats/StatsActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"
#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Landmarks/LandmarksActions.h"
#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Terrain/TerrainActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

namespace {

using FRuntimeProjectionPredicate = TFunction<bool(const rtk::AnyAction &)>;

/**
 * @fn template <typename Payload> FRuntimeProjectionPredicate ProjectionTrigger(const rtk::ActionCreator<Payload> &Creator)
 * @brief Adapts one typed action creator into a projection predicate.
 * @param Creator Action creator whose type triggers ECS projection.
 * @return Predicate suitable for the projection trigger catalog.
 *
 * User Story: As a projection maintainer, I need one adapter from typed actions
 * to projection predicates so trigger declarations stay uniform and composable.
 */
template <typename Payload>
FRuntimeProjectionPredicate
ProjectionTrigger(const rtk::ActionCreator<Payload> &Creator) {
  return [Creator](const rtk::AnyAction &Action) {
    return Creator.match(Action);
  };
}

/**
 * @fn const TArray<FRuntimeProjectionPredicate> &ProjectionTriggers()
 * @brief Declares every action that invalidates the projected ECS world.
 * @return Stable projection predicates evaluated in declaration order.
 *
 * User Story: As a projection maintainer, I need trigger membership declared as
 * data so extending a domain does not deepen a boolean control-flow chain.
 */
const TArray<FRuntimeProjectionPredicate> &ProjectionTriggers() {
  static const TArray<FRuntimeProjectionPredicate> Triggers = {
      ProjectionTrigger(RuntimeActions::RuntimeHydrated()),
      ProjectionTrigger(PlayerActions::PlayerObserved()),
      ProjectionTrigger(TerrainActions::TerrainLoaded()),
      ProjectionTrigger(SpawnActions::PlayerSpawnAnchored()),
      ProjectionTrigger(
          InteractionActions::TownspersonCandidatesObserved()),
      ProjectionTrigger(LandmarkActions::LandmarksSeeded()),
      ProjectionTrigger(NatureActions::NatureSeeded()),
      ProjectionTrigger(TownspersonActions::TownspeopleSeeded()),
      ProjectionTrigger(HorseActions::HorsesSeeded()),
      ProjectionTrigger(BotActions::BotsSeeded()),
      ProjectionTrigger(BotActions::BotUpserted()),
      ProjectionTrigger(BotStatsActions::BotStatsSeeded()),
      ProjectionTrigger(BotStatsActions::BotStatsUpdated()),
      ProjectionTrigger(BotPositionActions::BotPositionsSeeded()),
      ProjectionTrigger(BotPositionActions::BotPositionUpserted()),
      ProjectionTrigger(BotPositionActions::BotPositionMoved()),
      ProjectionTrigger(BotAIActions::BotAISeeded()),
      ProjectionTrigger(BotAIActions::BotAIUpdated()),
      ProjectionTrigger(BotGoalActions::BotGoalsSeeded()),
      ProjectionTrigger(BotGoalActions::BotGoalAssigned()),
      ProjectionTrigger(BotGoalActions::BotGoalCompleted())};
  return Triggers;
}

} // namespace

/** User Story: As a features systems projection consumer, I need to invoke reduce runtime projected through a stable signature so the features systems projection workflow remains explicit and composable. @fn FRuntimeState ReduceRuntimeProjected(const FRuntimeState &State) */
FRuntimeState ReduceRuntimeProjected(const FRuntimeState &State) {
  return (func::pipe(State) |
          [](FRuntimeState Next) -> FRuntimeState {
            Next.Ecs.World =
                SystemsAdapters::ProjectRuntimeWorld({Next});
            return Next;
          })
      .val;
}

/** User Story: As a features systems projection consumer, I need to invoke should project runtime action through a stable signature so the features systems projection workflow remains explicit and composable. @fn bool ShouldProjectRuntimeAction(const rtk::AnyAction &Action) */
bool ShouldProjectRuntimeAction(const rtk::AnyAction &Action) {
  return func::any_array<FRuntimeProjectionPredicate>(
      ProjectionTriggers(), [&Action](const FRuntimeProjectionPredicate &Match) {
        return Match(Action);
      });
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
