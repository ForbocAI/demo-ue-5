#include "Features/Systems/Bots/AI/BotAISlice.h"

#include "Features/Systems/Bots/AI/BotAIReducers.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotAISlice {

const rtk::Slice<FBotAIState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotAIState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotAIState> {
        return rtk::createSlice<FBotAIState>(
      TEXT("botAI"), BotAIFactories::CreateInitialState(),
      [](rtk::ActionReducerMapBuilder<FBotAIState> &Builder) {
    Builder.addCase(BotAIActions::BotAISeeded(),
                                BotAIReducers::ReduceBotAISeeded);
    Builder.addCase(BotAIActions::BotAIUpdated(),
                                BotAIReducers::ReduceBotAIUpdated);
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                BotAIReducers::ReduceTownspeopleSeeded);
    Builder.addCase(HorseActions::HorsesSeeded(),
                                BotAIReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace BotAISlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
