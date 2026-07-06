#include "Features/Systems/Bots/Goals/BotGoalSlice.h"

#include "Features/Systems/Bots/Goals/BotGoalReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalSlice {

const rtk::Slice<FBotGoalState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotGoalState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotGoalState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotGoalState>(
          TEXT("botGoals"), BotGoalFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FBotGoalState> &Builder) {
    Builder.addCase(BotGoalActions::BotGoalsSeeded(),
                                BotGoalReducers::ReduceBotGoalsSeeded);
    Builder.addCase(BotGoalActions::BotGoalAssigned(),
                                BotGoalReducers::ReduceBotGoalAssigned);
    Builder.addCase(BotGoalActions::BotGoalCompleted(),
                                BotGoalReducers::ReduceBotGoalCompleted);
  });
      });
  return func::eval(Slice);
}

} // namespace BotGoalSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
