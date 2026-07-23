#include "Features/Systems/Bots/SystemsBotsSlice.h"

#include "Features/Systems/Bots/SystemsBotsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotReducers {

/** User Story: As a features systems bots consumer, I need to invoke reduce bots seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceBotsSeeded( const FBotState &State, const rtk::PayloadAction<TArray<FBotEntity>> &Action) */
FBotState ReduceBotsSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FBotEntity>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().setAll(State.Items,
                                                Action.PayloadValue);
  return Next;
  }).val;
}

/** User Story: As a features systems bots consumer, I need to invoke reduce bot upserted through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceBotUpserted(const FBotState &State, const rtk::PayloadAction<FBotEntity> &Action) */
FBotState ReduceBotUpserted(const FBotState &State,
                            const rtk::PayloadAction<FBotEntity> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items =
      BotAdapters::BotAdapter().upsertOne(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

/** User Story: As a features systems bots consumer, I need to invoke reduce townspeople seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceTownspeopleSeeded( const FBotState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().upsertMany(
      State.Items, BotFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

/** User Story: As a features systems bots consumer, I need to invoke reduce horses seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceHorsesSeeded( const FBotState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
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

#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSlice {

/** User Story: As a features systems bots consumer, I need to invoke get slice through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::Slice<FBotState> &GetSlice() */
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
#include "Features/Systems/Bots/SystemsBotsAdapters.h"
#include "Features/Systems/Bots/Source/Mapping/MappingAdapters.h"

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

/** User Story: As a features systems bots consumer, I need to invoke entity source through a stable signature so the features systems bots workflow remains explicit and composable. @fn template <typename Seed> FBotEntitySource EntitySource(const Seed &SeedValue, const FBotEntityDefaults &Defaults) */
template <typename Seed>
FBotEntitySource EntitySource(const Seed &SeedValue,
                              const FBotEntityDefaults &Defaults) {
  return {SeedValue.Id, SeedValue.Name, Defaults.Kind, Defaults.Alignment,
          Defaults.bActive};
}

/** User Story: As a features systems bots consumer, I need to invoke from seeds through a stable signature so the features systems bots workflow remains explicit and composable. @fn template <typename Seed> TArray<FBotEntity> FromSeeds(const TArray<Seed> &Seeds, const FBotEntityDefaults &Defaults) */
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

/** User Story: As a features systems bots consumer, I need to invoke create initial state through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState CreateInitialState() */
FBotState CreateInitialState() {
  FBotState State;
  State.Items = BotAdapters::BotAdapter().getInitialState();
  return State;
}

/** User Story: As a features systems bots consumer, I need to invoke bot through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotEntity Bot(const FBotEntitySource &Source) */
FBotEntity Bot(const FBotEntitySource &Source) {
  FBotEntity Result;
  Result.Id = Source.Id;
  Result.DisplayName = Source.DisplayName;
  Result.Kind = Source.Kind;
  Result.Alignment = Source.Alignment;
  Result.bActive = Source.bActive;
  return Result;
}

/** User Story: As a features systems bots consumer, I need to invoke from townspeople through a stable signature so the features systems bots workflow remains explicit and composable. @fn TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) */
TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return FromSeeds<FTownspersonSeed>(Seeds, TownspersonEntityDefaults);
}

/** User Story: As a features systems bots consumer, I need to invoke from horses through a stable signature so the features systems bots workflow remains explicit and composable. @fn TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) */
TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromSeeds<FHorseRouteSeed>(Seeds, HorseEntityDefaults);
}

} // namespace BotFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
