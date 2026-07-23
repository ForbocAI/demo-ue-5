#pragma once

#include "Core/fp.hpp"
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

/** User Story: As a systems bots mapping consumer, I need to invoke local origin through a stable signature so the systems bots mapping workflow remains explicit and composable. @fn inline FLevelLocalPoint LocalOrigin() */
inline FLevelLocalPoint LocalOrigin() {
  return FLevelLocalPoint{0.0f, 0.0f, 0.0f};
}

/** User Story: As a systems bots mapping consumer, I need to invoke initial settings point through a stable signature so the systems bots mapping workflow remains explicit and composable. @fn template <typename SettingsValue> FLevelLocalPoint InitialSettingsPoint(const SettingsValue &Settings) */
template <typename SettingsValue>
FLevelLocalPoint InitialSettingsPoint(const SettingsValue &Settings) {
  return FLevelLocalPoint{static_cast<float>(Settings.InitialPosition.X),
                          static_cast<float>(Settings.InitialPosition.Y),
                          static_cast<float>(Settings.InitialPosition.Z)};
}

/** User Story: As a systems bots mapping consumer, I need to invoke first route point or through a stable signature so the systems bots mapping workflow remains explicit and composable. @fn inline FLevelLocalPoint FirstRoutePointOr(const TArray<FLevelLocalPoint> &Route, const FLevelLocalPoint &DefaultPoint) */
inline FLevelLocalPoint
FirstRoutePointOr(const TArray<FLevelLocalPoint> &Route,
                  const FLevelLocalPoint &DefaultPoint) {
  return Route.IsEmpty() ? DefaultPoint : Route[0];
}

/** User Story: As a systems bots mapping consumer, I need to invoke first route point through a stable signature so the systems bots mapping workflow remains explicit and composable. @fn inline FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route) */
inline FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route) {
  return FirstRoutePointOr(Route, LocalOrigin());
}

/** User Story: As a systems bots mapping consumer, I need to invoke first route point through a stable signature so the systems bots mapping workflow remains explicit and composable. @fn template <typename SettingsValue> FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route, const SettingsValue &Settings) */
template <typename SettingsValue>
FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route,
                                 const SettingsValue &Settings) {
  return FirstRoutePointOr(Route, InitialSettingsPoint(Settings));
}

/** User Story: As a systems bots mapping consumer, I need to invoke map seed components through a stable signature so the systems bots mapping workflow remains explicit and composable. @fn template <typename Seed, typename Source, typename Component> TArray<Component> MapSeedComponents(const TArray<Seed> &Seeds, TFunctionRef<Source(const Seed &)> BuildSourceValue, TFunctionRef<Component(const Source &)> BuildComponentValue) */
template <typename Seed, typename Source, typename Component>
TArray<Component> MapSeedComponents(const TArray<Seed> &Seeds,
                                    TFunctionRef<Source(const Seed &)> BuildSourceValue,
                                    TFunctionRef<Component(const Source &)> BuildComponentValue) {
  return func::map_array<Seed, Component>(
      Seeds, [BuildSourceValue, BuildComponentValue](const Seed &SeedValue) {
        return BuildComponentValue(BuildSourceValue(SeedValue));
      });
}

/** User Story: As a systems bots mapping consumer, I need to invoke map seed settings components through a stable signature so the systems bots mapping workflow remains explicit and composable. @fn template <typename Request, typename Seed, typename Source, typename Component> TArray<Component> MapSeedSettingsComponents( const Request &RequestValue, TFunctionRef<Source(const decltype(RequestValue.Settings) &, const Seed &)> BuildSourceValue, TFunctionRef<Component(const Source &)> BuildComponentValue) */
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
