#include "Features/Systems/Projection/Slice.h"

#include "Features/Entities/Characters/Player/Actions.h"
#include "Features/Systems/Bots/AI/Actions.h"
#include "Features/Systems/Bots/Actions.h"
#include "Features/Systems/Bots/Goals/Actions.h"
#include "Features/Systems/Bots/Horses/Actions.h"
#include "Features/Systems/Bots/Position/Actions.h"
#include "Features/Systems/Bots/Stats/Actions.h"
#include "Features/Systems/Bots/Townspeople/Actions.h"
#include "Features/Systems/Interaction/Actions.h"
#include "Features/Systems/Landmarks/Actions.h"
#include "Features/Systems/Nature/Actions.h"
#include "Features/Systems/Actions.h"
#include "Features/Systems/Spawn/Actions.h"
#include "Features/Systems/Adapters.h"
#include "Features/Systems/Terrain/Actions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

namespace {

template <typename Payload>
bool RuntimeActionMatches(const rtk::AnyAction &Action,
                          const rtk::ActionCreator<Payload> &Creator) {
  return Creator.match(Action);
}

} // namespace

FRuntimeState ReduceRuntimeProjected(const FRuntimeState &State) {
  return (func::pipe(State) |
          [](FRuntimeState Next) -> FRuntimeState {
            Next.Ecs.World =
                SystemsAdapters::ProjectRuntimeWorld({Next});
            return Next;
          })
      .val;
}

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
