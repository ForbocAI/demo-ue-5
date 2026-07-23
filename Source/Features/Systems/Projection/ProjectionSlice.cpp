#include "Features/Systems/Projection/ProjectionSlice.h"

#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Systems/Bots/AI/AIActions.h"
#include "Features/Systems/Bots/SystemsBotsActions.h"
#include "Features/Systems/Bots/Goals/GoalsActions.h"
#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/Bots/Stats/StatsActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"
#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"
#include "Features/Systems/Nature/SystemsNatureActions.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Terrain/TerrainActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

namespace {

/** User Story: As a features systems projection consumer, I need to invoke runtime action matches through a stable signature so the features systems projection workflow remains explicit and composable. @fn template <typename Payload> bool RuntimeActionMatches(const rtk::AnyAction &Action, const rtk::ActionCreator<Payload> &Creator) */
template <typename Payload>
bool RuntimeActionMatches(const rtk::AnyAction &Action,
                          const rtk::ActionCreator<Payload> &Creator) {
  return Creator.match(Action);
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
  return RuntimeActionMatches(Action, RuntimeActions::RuntimeHydrated()) ||
         RuntimeActionMatches(Action, PlayerActions::PlayerObserved()) ||
         RuntimeActionMatches(Action, TerrainActions::TerrainLoaded()) ||
         RuntimeActionMatches(Action, SpawnActions::PlayerSpawnAnchored()) ||
         RuntimeActionMatches(
             Action, InteractionActions::TownspersonCandidatesObserved()) ||
         RuntimeActionMatches(Action, LandmarkActions::LandmarksSeeded()) ||
         RuntimeActionMatches(Action, NatureActions::NatureSeeded()) ||
         RuntimeActionMatches(Action, TownspersonActions::TownspeopleSeeded()) ||
         RuntimeActionMatches(Action, HorseActions::HorsesSeeded()) ||
         RuntimeActionMatches(Action, BotActions::BotsSeeded()) ||
         RuntimeActionMatches(Action, BotActions::BotUpserted()) ||
         RuntimeActionMatches(Action, BotStatsActions::BotStatsSeeded()) ||
         RuntimeActionMatches(Action, BotStatsActions::BotStatsUpdated()) ||
         RuntimeActionMatches(Action, BotPositionActions::BotPositionsSeeded()) ||
         RuntimeActionMatches(Action,
                              BotPositionActions::BotPositionUpserted()) ||
         RuntimeActionMatches(Action, BotPositionActions::BotPositionMoved()) ||
         RuntimeActionMatches(Action, BotAIActions::BotAISeeded()) ||
         RuntimeActionMatches(Action, BotAIActions::BotAIUpdated()) ||
         RuntimeActionMatches(Action, BotGoalActions::BotGoalsSeeded()) ||
         RuntimeActionMatches(Action, BotGoalActions::BotGoalAssigned()) ||
         RuntimeActionMatches(Action, BotGoalActions::BotGoalCompleted());
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
