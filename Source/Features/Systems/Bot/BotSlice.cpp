#include "Features/Systems/Bot/BotSlice.h"

#include "Core/frmt.hpp"
#include "Features/Systems/Bots/Stats/StatsSlice.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceRuntimeTownspeopleSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.BotStats = BotStatsReducers::ReduceTownspeopleSeeded(
                {Next.BotStats, Action.PayloadValue, Next.Bot});
            Next.BotGoals = BotGoalReducers::ReduceTownspeopleSeeded(
                {Next.BotGoals, Action.PayloadValue, Next.Bot});
            return Next;
          })
      .val;
}

FRuntimeState ReduceRuntimeHorsesSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.BotStats = BotStatsReducers::ReduceHorsesSeeded(
                {Next.BotStats, Action.PayloadValue, Next.Bot});
            Next.BotGoals = BotGoalReducers::ReduceHorsesSeeded(
                {Next.BotGoals, Action.PayloadValue, Next.Bot});
            return Next;
          })
      .val;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
