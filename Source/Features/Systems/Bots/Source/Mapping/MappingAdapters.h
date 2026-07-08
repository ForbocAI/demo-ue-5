#pragma once

#include "Core/ue_fp.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSourceMappingAdapters {

template <typename Seed, typename SettingsValue>
struct TSeedSettingsRequest {
  TArray<Seed> Seeds;
  SettingsValue Settings;
};

inline FLevelLocalPoint LocalOrigin() {
  return FLevelLocalPoint{0.0f, 0.0f, 0.0f};
}

template <typename SettingsValue>
FLevelLocalPoint InitialSettingsPoint(const SettingsValue &Settings) {
  return FLevelLocalPoint{static_cast<float>(Settings.InitialPosition.X),
                          static_cast<float>(Settings.InitialPosition.Y),
                          static_cast<float>(Settings.InitialPosition.Z)};
}

inline FLevelLocalPoint
FirstRoutePointOr(const TArray<FLevelLocalPoint> &Route,
                  const FLevelLocalPoint &DefaultPoint) {
  return Route.IsEmpty() ? DefaultPoint : Route[0];
}

inline FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route) {
  return FirstRoutePointOr(Route, LocalOrigin());
}

template <typename SettingsValue>
FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route,
                                 const SettingsValue &Settings) {
  return FirstRoutePointOr(Route, InitialSettingsPoint(Settings));
}

template <typename Seed, typename Source, typename Component>
TArray<Component> MapSeedComponents(const TArray<Seed> &Seeds,
                                    TFunctionRef<Source(const Seed &)> BuildSourceValue,
                                    TFunctionRef<Component(const Source &)> BuildComponentValue) {
  return func::map_array<Seed, Component>(
      Seeds, [BuildSourceValue, BuildComponentValue](const Seed &SeedValue) {
        return BuildComponentValue(BuildSourceValue(SeedValue));
      });
}

template <typename Request, typename Seed, typename Source, typename Component>
TArray<Component> MapSeedSettingsComponents(
    const Request &RequestValue,
    TFunctionRef<Source(const decltype(RequestValue.Settings) &, const Seed &)> BuildSourceValue,
    TFunctionRef<Component(const Source &)> BuildComponentValue) {
  return MapSeedComponents<Seed, Source, Component>(
      RequestValue.Seeds,
      [&RequestValue, BuildSourceValue](const Seed &SeedValue) {
        return BuildSourceValue(RequestValue.Settings, SeedValue);
      },
      BuildComponentValue);
}

} // namespace BotSourceMappingAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
