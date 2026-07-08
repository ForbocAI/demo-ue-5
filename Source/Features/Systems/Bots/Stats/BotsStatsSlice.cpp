#include "Features/Systems/Bots/Stats/StatsSlice.h"

#include "Features/Systems/Bots/Stats/BotsStatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsReducers {

FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action) {
  return (func::pipe(State) | [&](FBotStatsState Next) -> FBotStatsState {
  Next.Items = BotStatsAdapters::BotStatsAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

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

bool NoMountedRider(const FTownspersonSeed &) { return false; }

bool HorseMountedRider(const FHorseRouteSeed &Seed) {
  return Seed.bMountedRider;
}

const TBotStatsDefaults<FTownspersonSeed> TownspersonStatsDefaults = {
    &Data::FBotSettings::TownspersonStats, NoMountedRider};

const TBotStatsDefaults<FHorseRouteSeed> HorseStatsDefaults = {
    &Data::FBotSettings::HorseStats, HorseMountedRider};

template <typename Seed>
const Data::FStatPresetSettings &
StatsPreset(const TBotStatsSourceRequest<Seed> &Request) {
  return Request.Settings.*(Request.Defaults.Preset);
}

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

FBotStatsState CreateInitialState() {
  FBotStatsState State;
  State.Items = BotStatsAdapters::BotStatsAdapter().getInitialState();
  return State;
}

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

TArray<FBotStatsComponent>
FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request) {
  return FromSeeds<FTownspersonSeed>(Request, TownspersonStatsDefaults);
}

TArray<FBotStatsComponent> FromHorses(
    const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request) {
  return FromSeeds<FHorseRouteSeed>(Request, HorseStatsDefaults);
}

} // namespace BotStatsFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
