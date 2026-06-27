#include "Features/Systems/Bots/Goals/BotGoalSlice.h"

#include "Features/Systems/Bots/Goals/BotGoalReducers.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalSlice {

const rtk::Slice<FBotGoalState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotGoalState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotGoalState> {
        return rtk::createSlice<FBotGoalState>(
          TEXT("botGoals"), BotGoalFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FBotGoalState> &Builder) {
    Builder.addCase(BotGoalActions::BotGoalsSeeded(),
                                BotGoalReducers::ReduceBotGoalsSeeded);
    Builder.addCase(BotGoalActions::BotGoalAssigned(),
                                BotGoalReducers::ReduceBotGoalAssigned);
    Builder.addCase(BotGoalActions::BotGoalCompleted(),
                                BotGoalReducers::ReduceBotGoalCompleted);
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                BotGoalReducers::ReduceTownspeopleSeeded);
    Builder.addCase(HorseActions::HorsesSeeded(),
                                BotGoalReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace BotGoalSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
