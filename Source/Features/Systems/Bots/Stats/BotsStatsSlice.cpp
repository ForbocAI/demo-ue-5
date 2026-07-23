#include "Features/Systems/Bots/Stats/StatsSlice.h"

#include "Features/Systems/Bots/Stats/BotsStatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsReducers {

/** User Story: As a systems bots stats consumer, I need to invoke reduce bot stats seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceBotStatsSeeded( const FBotStatsState &State, const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action) */
FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action) {
  return (func::pipe(State) | [&](FBotStatsState Next) -> FBotStatsState {
  Next.Items = BotStatsAdapters::BotStatsAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

/** User Story: As a systems bots stats consumer, I need to invoke reduce bot stats updated through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceBotStatsUpdated( const FBotStatsState &State, const rtk::PayloadAction<FBotStatsUpdate> &Action) */
FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::PayloadAction<FBotStatsUpdate> &Action) {
  return (func::pipe(State) | [&](FBotStatsState Next) -> FBotStatsState {
  const FBotStatsUpdate Payload = Action.PayloadValue;
  Next.Items = BotStatsAdapters::BotStatsAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotStatsComponent &Current) -> FBotStatsComponent {
        FBotStatsComponent Updated = Current;
        Updated.MoveSpeed = Payload.MoveSpeed;
        Updated.AwarenessRange = Payload.AwarenessRange;
        Updated.Resolve = Payload.Resolve;
        return Updated;
      });
  return Next;
  }).val;
}

/** User Story: As a systems bots stats consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceTownspeopleSeeded( const FBotStatsTownspeopleSeededRequest &Request) */
FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsTownspeopleSeededRequest &Request) {
  return (func::pipe(Request.State) | [&](FBotStatsState Next) -> FBotStatsState {
  Next.Items = BotStatsAdapters::BotStatsAdapter().upsertMany(
      Request.State.Items,
      BotStatsFactories::FromTownspeople(
          {Request.Seeds, Request.Settings}));
  return Next;
  }).val;
}

/** User Story: As a systems bots stats consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceHorsesSeeded( const FBotStatsHorsesSeededRequest &Request) */
FBotStatsState ReduceHorsesSeeded(
    const FBotStatsHorsesSeededRequest &Request) {
  return (func::pipe(Request.State) | [&](FBotStatsState Next) -> FBotStatsState {
  Next.Items = BotStatsAdapters::BotStatsAdapter().upsertMany(
      Request.State.Items,
      BotStatsFactories::FromHorses({Request.Seeds,
                                     Request.Settings}));
  return Next;
  }).val;
}

} // namespace BotStatsReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsSlice {

/** User Story: As a systems bots stats consumer, I need to invoke get slice through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::Slice<FBotStatsState> &GetSlice() */
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
} // namespace Game
} // namespace ForbocAI


#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Stats/BotsStatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsFactories {
namespace {

template <typename Seed> struct TBotStatsDefaults {
  Data::FStatPresetSettings Data::FBotSettings::*Preset;
  bool (*SelectMountedRider)(const Seed &SeedValue);
};

template <typename Seed> struct TBotStatsSourceRequest {
  const Data::FBotSettings &Settings;
  const Seed &SeedValue;
  const TBotStatsDefaults<Seed> &Defaults;
};

/** User Story: As a systems bots stats consumer, I need to invoke no mounted rider through a stable signature so the systems bots stats workflow remains explicit and composable. @fn bool NoMountedRider(const FTownspersonSeed &) */
bool NoMountedRider(const FTownspersonSeed &) { return false; }

/** User Story: As a systems bots stats consumer, I need to invoke horse mounted rider through a stable signature so the systems bots stats workflow remains explicit and composable. @fn bool HorseMountedRider(const FHorseRouteSeed &Seed) */
bool HorseMountedRider(const FHorseRouteSeed &Seed) {
  return Seed.bMountedRider;
}

const TBotStatsDefaults<FTownspersonSeed> TownspersonStatsDefaults = {
    &Data::FBotSettings::TownspersonStats, NoMountedRider};

const TBotStatsDefaults<FHorseRouteSeed> HorseStatsDefaults = {
    &Data::FBotSettings::HorseStats, HorseMountedRider};

/** User Story: As a systems bots stats consumer, I need to invoke stats preset through a stable signature so the systems bots stats workflow remains explicit and composable. @fn template <typename Seed> const Data::FStatPresetSettings & StatsPreset(const TBotStatsSourceRequest<Seed> &Request) */
template <typename Seed>
const Data::FStatPresetSettings &
StatsPreset(const TBotStatsSourceRequest<Seed> &Request) {
  return Request.Settings.*(Request.Defaults.Preset);
}

/** User Story: As a systems bots stats consumer, I need to invoke stats source through a stable signature so the systems bots stats workflow remains explicit and composable. @fn template <typename Seed> FBotStatsSource StatsSource(const TBotStatsSourceRequest<Seed> &Request) */
template <typename Seed>
FBotStatsSource StatsSource(const TBotStatsSourceRequest<Seed> &Request) {
  const Data::FStatPresetSettings &Preset = StatsPreset<Seed>(Request);
  return {Request.SeedValue.Id,
          Preset.MoveSpeed,
          Preset.AwarenessRange,
          Preset.Resolve,
          Preset.bCanTalk,
          Request.Defaults.SelectMountedRider(Request.SeedValue)};
}

/** User Story: As a systems bots stats consumer, I need to invoke from seeds through a stable signature so the systems bots stats workflow remains explicit and composable. @fn template <typename Seed> TArray<FBotStatsComponent> FromSeeds(const TBotStatsFromSeedsRequest<Seed> &Request, const TBotStatsDefaults<Seed> &Defaults) */
template <typename Seed>
TArray<FBotStatsComponent>
FromSeeds(const TBotStatsFromSeedsRequest<Seed> &Request,
          const TBotStatsDefaults<Seed> &Defaults) {
  return BotSourceMappingAdapters::MapSeedSettingsComponents<
      TBotStatsFromSeedsRequest<Seed>, Seed, FBotStatsSource,
      FBotStatsComponent>(
      Request,
      [&Defaults](const Data::FBotSettings &Settings,
                  const Seed &SeedValue) {
        return StatsSource<Seed>({Settings, SeedValue, Defaults});
      },
      Component);
}

} // namespace

/** User Story: As a systems bots stats consumer, I need to invoke create initial state through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState CreateInitialState() */
FBotStatsState CreateInitialState() {
  FBotStatsState State;
  State.Items = BotStatsAdapters::BotStatsAdapter().getInitialState();
  return State;
}

/** User Story: As a systems bots stats consumer, I need to invoke component through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsComponent Component(const FBotStatsSource &Source) */
FBotStatsComponent Component(const FBotStatsSource &Source) {
  FBotStatsComponent Result;
  Result.Id = Source.Id;
  Result.MoveSpeed = Source.MoveSpeed;
  Result.AwarenessRange = Source.AwarenessRange;
  Result.Resolve = Source.Resolve;
  Result.bCanTalk = Source.bCanTalk;
  Result.bMountedRider = Source.bMountedRider;
  return Result;
}

/** User Story: As a systems bots stats consumer, I need to invoke from townspeople through a stable signature so the systems bots stats workflow remains explicit and composable. @fn TArray<FBotStatsComponent> FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request) */
TArray<FBotStatsComponent>
FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request) {
  return FromSeeds<FTownspersonSeed>(Request, TownspersonStatsDefaults);
}

/** User Story: As a systems bots stats consumer, I need to invoke from horses through a stable signature so the systems bots stats workflow remains explicit and composable. @fn TArray<FBotStatsComponent> FromHorses( const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request) */
TArray<FBotStatsComponent> FromHorses(
    const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request) {
  return FromSeeds<FHorseRouteSeed>(Request, HorseStatsDefaults);
}

} // namespace BotStatsFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
