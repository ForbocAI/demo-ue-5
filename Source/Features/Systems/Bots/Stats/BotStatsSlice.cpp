#include "Features/Systems/Bots/Stats/BotStatsSlice.h"

#include "Features/Systems/Bots/Stats/BotStatsReducers.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsSlice {

const rtk::Slice<FBotStatsState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotStatsState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotStatsState> {
        return rtk::createSlice<FBotStatsState>(
          TEXT("botStats"), BotStatsFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FBotStatsState> &Builder) {
    Builder.addCase(BotStatsActions::BotStatsSeeded(),
                                BotStatsReducers::ReduceBotStatsSeeded);
    Builder.addCase(BotStatsActions::BotStatsUpdated(),
                                BotStatsReducers::ReduceBotStatsUpdated);
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                BotStatsReducers::ReduceTownspeopleSeeded);
    Builder.addCase(HorseActions::HorsesSeeded(),
                                BotStatsReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace BotStatsSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
