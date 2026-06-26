#include "Features/Systems/Bots/Position/BotPositionSlice.h"

#include "Features/Systems/Bots/Position/BotPositionReducers.h"
#include "Features/Systems/Horses/HorseActions.h"
#include "Features/Systems/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionSlice {

const rtk::Slice<FBotPositionState> &GetSlice() {
  static const rtk::Slice<FBotPositionState> Slice = []() {
    rtk::SliceBuilder<FBotPositionState> Builder =
        rtk::sliceBuilder<FBotPositionState>(
            TEXT("botPosition"), BotPositionFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder,
                                BotPositionActions::BotPositionsSeeded(),
                                BotPositionReducers::ReduceBotPositionsSeeded);
    Builder = rtk::addExtraCase(Builder,
                                BotPositionActions::BotPositionMoved(),
                                BotPositionReducers::ReduceBotPositionMoved);
    Builder = rtk::addExtraCase(Builder,
                                TownspersonActions::TownspeopleSeeded(),
                                BotPositionReducers::ReduceTownspeopleSeeded);
    Builder = rtk::addExtraCase(Builder, HorseActions::HorsesSeeded(),
                                BotPositionReducers::ReduceHorsesSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace BotPositionSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
