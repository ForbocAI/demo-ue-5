#include "Features/Systems/Bots/Slice.h"

#include "Features/Systems/Bots/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotReducers {

FBotState ReduceBotsSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FBotEntity>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().setAll(State.Items,
                                                Action.PayloadValue);
  return Next;
  }).val;
}

FBotState ReduceBotUpserted(const FBotState &State,
                            const rtk::PayloadAction<FBotEntity> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items =
      BotAdapters::BotAdapter().upsertOne(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().upsertMany(
      State.Items, BotFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

FBotState ReduceHorsesSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().upsertMany(
      State.Items, BotFactories::FromHorses(Action.PayloadValue));
  return Next;
  }).val;
}

} // namespace BotReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Horses/Actions.h"
#include "Features/Systems/Bots/Townspeople/Actions.h"

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


#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Adapters.h"
#include "Features/Systems/Bots/Source/Mapping/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotFactories {
namespace {

struct FBotEntityDefaults {
  EBotEntityKind Kind;
  EBotAlignment Alignment;
  bool bActive;
};

const FBotEntityDefaults TownspersonEntityDefaults = {
    EBotEntityKind::Townsperson, EBotAlignment::Friendly, true};

const FBotEntityDefaults HorseEntityDefaults = {
    EBotEntityKind::Horse, EBotAlignment::Neutral, true};

template <typename Seed>
FBotEntitySource EntitySource(const Seed &SeedValue,
                              const FBotEntityDefaults &Defaults) {
  return {SeedValue.Id, SeedValue.Name, Defaults.Kind, Defaults.Alignment,
          Defaults.bActive};
}

template <typename Seed>
TArray<FBotEntity> FromSeeds(const TArray<Seed> &Seeds,
                             const FBotEntityDefaults &Defaults) {
  return BotSourceMappingAdapters::MapSeedComponents<Seed, FBotEntitySource,
                                             FBotEntity>(
      Seeds,
      [&Defaults](const Seed &SeedValue) {
        return EntitySource<Seed>(SeedValue, Defaults);
      },
      Bot);
}

} // namespace

FBotState CreateInitialState() {
  FBotState State;
  State.Items = BotAdapters::BotAdapter().getInitialState();
  return State;
}

FBotEntity Bot(const FBotEntitySource &Source) {
  FBotEntity Result;
  Result.Id = Source.Id;
  Result.DisplayName = Source.DisplayName;
  Result.Kind = Source.Kind;
  Result.Alignment = Source.Alignment;
  Result.bActive = Source.bActive;
  return Result;
}

TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return FromSeeds<FTownspersonSeed>(Seeds, TownspersonEntityDefaults);
}

TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromSeeds<FHorseRouteSeed>(Seeds, HorseEntityDefaults);
}

} // namespace BotFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
