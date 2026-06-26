#include "Features/Systems/Bots/BotSlice.h"

#include "Features/Systems/Bots/BotReducers.h"
#include "Features/Systems/Horses/HorseActions.h"
#include "Features/Systems/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotSlice {

const rtk::Slice<FBotState> &GetSlice() {
  static const rtk::Slice<FBotState> Slice = []() {
    rtk::SliceBuilder<FBotState> Builder =
        rtk::sliceBuilder<FBotState>(TEXT("bots"),
                                     BotFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, BotActions::BotsSeeded(),
                                BotReducers::ReduceBotsSeeded);
    Builder = rtk::addExtraCase(Builder,
                                TownspersonActions::TownspeopleSeeded(),
                                BotReducers::ReduceTownspeopleSeeded);
    Builder = rtk::addExtraCase(Builder, HorseActions::HorsesSeeded(),
                                BotReducers::ReduceHorsesSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace BotSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
