#include "Features/Systems/Bots/BotSlice.h"

#include "Features/Systems/Bots/BotReducers.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSlice {

const rtk::Slice<FBotState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotState>(
      TEXT("bots"), BotFactories::CreateInitialState(),
      [](rtk::ActionReducerMapBuilder<FBotState> &Builder) {
    Builder.addCase(BotActions::BotsSeeded(),
                                BotReducers::ReduceBotsSeeded);
    Builder.addCase(BotActions::BotUpserted(),
                                BotReducers::ReduceBotUpserted);
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                BotReducers::ReduceTownspeopleSeeded);
    Builder.addCase(HorseActions::HorsesSeeded(),
                                BotReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace BotSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
