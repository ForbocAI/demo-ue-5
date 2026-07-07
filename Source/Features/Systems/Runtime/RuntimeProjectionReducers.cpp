#include "Features/Systems/Runtime/RuntimeProjectionReducers.h"

#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Systems/Bots/AI/BotAIActions.h"
#include "Features/Systems/Bots/BotActions.h"
#include "Features/Systems/Bots/Goals/BotGoalActions.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Bots/Stats/BotStatsActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Landmarks/LandmarkActions.h"
#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Terrain/TerrainActions.h"

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
