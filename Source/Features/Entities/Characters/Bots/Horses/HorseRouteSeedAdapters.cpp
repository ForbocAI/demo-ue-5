#include "Features/Entities/Characters/Bots/BotsAdapters.h"

#include "Features/Components/Data/Json/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

struct FHorseRoutePointFields {
  float EastLots;
  float NorthLots;
};

struct FHorseRouteFields {
  FString Id;
  FString Name;
  bool bMountedRider;
  TArray<TSharedPtr<FJsonValue>> PatrolRoute;
};

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace BotSeedTypes = ForbocAI::Game::Level::BotsAdapters;

using FHorseRouteFields = BotSeedTypes::FHorseRouteFields;
using FHorseRoutePointFields = BotSeedTypes::FHorseRoutePointFields;

JSON_SETTINGS_REGISTRY(FHorseRoutePointFields, EastLots, NorthLots);
JSON_SETTINGS_REGISTRY(FHorseRouteFields, Id, Name, bMountedRider,
                       PatrolRoute);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {
namespace {

struct FRouteLotsRequest {
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
};

typedef FLevelLocalPoint (*FRouteLotsProjector)(const FRouteLotsRequest &);

struct FPatrolRouteFieldsRequest {
  TArray<TSharedPtr<FJsonValue>> Points;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FRouteLotsProjector ProjectLots;
};

struct FHorseRouteBuildRequest {
  FHorseRouteFields Fields;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

FLevelLocalPoint HorseRouteLots(const FRouteLotsRequest &Request) {
  return LevelLayoutSlice::FromPostOfficeLots(
      {Request.Geometry, Request.EastLots, Request.NorthLots,
       LevelLayoutSlice::RoadSurfaceClearance(Request.Geometry)});
}

TArray<FLevelLocalPoint>
PatrolRouteFromFields(const FPatrolRouteFieldsRequest &Request) {
  const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
      Request.Geometry;
  const FRouteLotsProjector ProjectLots = Request.ProjectLots;
  return func::map_array<FHorseRoutePointFields, FLevelLocalPoint>(
      JsonAdapters::MapSettingsJsonValues<FHorseRoutePointFields>(
          Request.Points, JSON_SETTINGS_ATOMS(EastLots, NorthLots)),
      [Geometry, ProjectLots](const FHorseRoutePointFields &Fields) {
        return ProjectLots({Geometry, Fields.EastLots, Fields.NorthLots});
      });
}

FHorseRouteSeed HorseRouteFromFields(
    const FHorseRouteBuildRequest &Request) {
  FHorseRouteSeed Seed;
  Seed.Id = Request.Fields.Id;
  Seed.Name = Request.Fields.Name;
  Seed.bMountedRider = Request.Fields.bMountedRider;
  Seed.PatrolRoute = PatrolRouteFromFields(
      {Request.Fields.PatrolRoute, Request.Geometry, HorseRouteLots});
  return Seed;
}

} // namespace

TArray<FHorseRouteSeed> BuildHorseRouteSeed(
    const FBotSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FHorseRouteFields, FHorseRouteSeed>(
      JsonAdapters::ReadSettingsObjectArrayField<FHorseRouteFields>(
          "Horses",
          JSON_SETTINGS_ATOMS(Id, Name, bMountedRider, PatrolRoute))(Root),
      [&Request](const FHorseRouteFields &Fields) {
        return HorseRouteFromFields({Fields, Request.Geometry});
      });
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentSourceValueFieldRegistry<FHorseRouteSeed> {
  static const TArray<TComponentSourceValueFieldDeclaration<FHorseRouteSeed>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FHorseRouteSeed>>
        RegisteredFields = {{"Id", &FHorseRouteSeed::Id},
                            {"Name", &FHorseRouteSeed::Name},
                            {"MountedRider", &FHorseRouteSeed::bMountedRider},
                            {"PatrolRoute", &FHorseRouteSeed::PatrolRoute}};
    return RegisteredFields;
  }
};

template <>
struct TComponentSourceProjector<FHorseRouteSeed> {
  ecs::FComponentValue operator()(const FHorseRouteSeed &Horse) const {
    return ComponentSourceValueMap(
        Horse, {"Id", "Name", "MountedRider", "PatrolRoute"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

extern ecs::EntityKey BotEntityKey(const FString &Id);

ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectHorseEntityPayload &PayloadValue) {
            return BotEntityKey(PayloadValue.Horse.Id);
          },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Characters", "Bots", "Horses"},
                   {"Systems", "Bots", "Horses"}})),
          [](const FProjectHorseEntityPayload &PayloadValue)
              -> const FHorseRouteSeed & {
            return PayloadValue.Horse;
          },
          RegisteredComponentGroups<FHorseRouteSeed>(
              {{"Components/Data", {"Id", "Name"}},
               {"Components/Bots", {"MountedRider"}},
               {"Components/Spatial", {"PatrolRoute"}}})});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
