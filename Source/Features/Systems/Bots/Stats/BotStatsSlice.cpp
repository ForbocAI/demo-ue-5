#include "Features/Systems/Bots/Stats/BotStatsSlice.h"

#include "Features/Systems/Bots/Stats/BotStatsReducers.h"
#include "Features/Systems/Horses/HorseActions.h"
#include "Features/Systems/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsSlice {

const rtk::Slice<FBotStatsState> &GetSlice() {
  static const rtk::Slice<FBotStatsState> Slice = []() {
    rtk::SliceBuilder<FBotStatsState> Builder =
        rtk::sliceBuilder<FBotStatsState>(
            TEXT("botStats"), BotStatsFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, BotStatsActions::BotStatsSeeded(),
                                BotStatsReducers::ReduceBotStatsSeeded);
    Builder = rtk::addExtraCase(Builder, BotStatsActions::BotStatsUpdated(),
                                BotStatsReducers::ReduceBotStatsUpdated);
    Builder = rtk::addExtraCase(Builder,
                                TownspersonActions::TownspeopleSeeded(),
                                BotStatsReducers::ReduceTownspeopleSeeded);
    Builder = rtk::addExtraCase(Builder, HorseActions::HorsesSeeded(),
                                BotStatsReducers::ReduceHorsesSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace BotStatsSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
