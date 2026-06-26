#include "Features/Systems/Bots/Goals/BotGoalSlice.h"

#include "Features/Systems/Bots/Goals/BotGoalReducers.h"
#include "Features/Systems/Horses/HorseActions.h"
#include "Features/Systems/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalSlice {

const rtk::Slice<FBotGoalState> &GetSlice() {
  static const rtk::Slice<FBotGoalState> Slice = []() {
    rtk::SliceBuilder<FBotGoalState> Builder =
        rtk::sliceBuilder<FBotGoalState>(
            TEXT("botGoals"), BotGoalFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, BotGoalActions::BotGoalsSeeded(),
                                BotGoalReducers::ReduceBotGoalsSeeded);
    Builder = rtk::addExtraCase(Builder, BotGoalActions::BotGoalAssigned(),
                                BotGoalReducers::ReduceBotGoalAssigned);
    Builder = rtk::addExtraCase(Builder, BotGoalActions::BotGoalCompleted(),
                                BotGoalReducers::ReduceBotGoalCompleted);
    Builder = rtk::addExtraCase(Builder,
                                TownspersonActions::TownspeopleSeeded(),
                                BotGoalReducers::ReduceTownspeopleSeeded);
    Builder = rtk::addExtraCase(Builder, HorseActions::HorsesSeeded(),
                                BotGoalReducers::ReduceHorsesSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace BotGoalSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
