#include "Features/Systems/Bots/Stats/BotStatsSlice.h"

#include "Features/Systems/Bots/Stats/BotStatsReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsSlice {

const rtk::Slice<FBotStatsState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotStatsState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotStatsState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotStatsState>(
          TEXT("botStats"), BotStatsFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FBotStatsState> &Builder) {
    Builder.addCase(BotStatsActions::BotStatsSeeded(),
                                BotStatsReducers::ReduceBotStatsSeeded);
    Builder.addCase(BotStatsActions::BotStatsUpdated(),
                                BotStatsReducers::ReduceBotStatsUpdated);
  });
      });
  return func::eval(Slice);
}

} // namespace BotStatsSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
