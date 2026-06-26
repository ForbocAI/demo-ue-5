#include "Features/Systems/Bots/AI/BotAISlice.h"

#include "Features/Systems/Bots/AI/BotAIReducers.h"
#include "Features/Systems/Horses/HorseActions.h"
#include "Features/Systems/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAISlice {

const rtk::Slice<FBotAIState> &GetSlice() {
  static const rtk::Slice<FBotAIState> Slice = []() {
    rtk::SliceBuilder<FBotAIState> Builder =
        rtk::sliceBuilder<FBotAIState>(TEXT("botAI"),
                                       BotAIFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, BotAIActions::BotAISeeded(),
                                BotAIReducers::ReduceBotAISeeded);
    Builder = rtk::addExtraCase(Builder, BotAIActions::BotAIUpdated(),
                                BotAIReducers::ReduceBotAIUpdated);
    Builder = rtk::addExtraCase(Builder,
                                TownspersonActions::TownspeopleSeeded(),
                                BotAIReducers::ReduceTownspeopleSeeded);
    Builder = rtk::addExtraCase(Builder, HorseActions::HorsesSeeded(),
                                BotAIReducers::ReduceHorsesSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace BotAISlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
