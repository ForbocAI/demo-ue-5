#include "Features/Systems/Bots/Position/BotPositionSlice.h"

#include "Features/Systems/Bots/Position/BotPositionReducers.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionSlice {

const rtk::Slice<FBotPositionState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotPositionState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotPositionState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotPositionState>(
          TEXT("botPosition"), BotPositionFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FBotPositionState> &Builder) {
    Builder.addCase(BotPositionActions::BotPositionsSeeded(),
                                BotPositionReducers::ReduceBotPositionsSeeded);
    Builder.addCase(BotPositionActions::BotPositionUpserted(),
                                BotPositionReducers::ReduceBotPositionUpserted);
    Builder.addCase(BotPositionActions::BotPositionMoved(),
                                BotPositionReducers::ReduceBotPositionMoved);
    Builder.addCase(
        BotPositionActions::InitialPatrolObserved(),
        BotPositionReducers::ReduceInitialPatrolObserved);
    Builder.addCase(
        BotPositionActions::PatrolAdvanceObserved(),
        BotPositionReducers::ReducePatrolAdvanceObserved);
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                BotPositionReducers::ReduceTownspeopleSeeded);
    Builder.addCase(HorseActions::HorsesSeeded(),
                                BotPositionReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace BotPositionSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
