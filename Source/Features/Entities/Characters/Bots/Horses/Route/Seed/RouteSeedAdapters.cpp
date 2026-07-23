#include "Features/Entities/Characters/Bots/CharactersBotsAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

struct FHorseRoutePointSource {
  float EastLots;
  float NorthLots;
};

struct FHorseRouteSource {
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

using FHorseRouteSource = BotSeedTypes::FHorseRouteSource;
using FHorseRoutePointSource = BotSeedTypes::FHorseRoutePointSource;

JSON_SETTINGS_REGISTRY(FHorseRoutePointSource, EastLots, NorthLots);
JSON_SETTINGS_REGISTRY(FHorseRouteSource, Id, Name, bMountedRider,
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
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
};

typedef FLevelLocalPoint (*FRouteLotsProjector)(const FRouteLotsRequest &);

struct FPatrolRouteFieldsRequest {
  TArray<TSharedPtr<FJsonValue>> Points;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FRouteLotsProjector ProjectLots;
};

struct FHorseRouteBuildRequest {
  FHorseRouteSource Fields;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

/** User Story: As a horses route seed consumer, I need to invoke horse route lots through a stable signature so the horses route seed workflow remains explicit and composable. @fn FLevelLocalPoint HorseRouteLots(const FRouteLotsRequest &Request) */
FLevelLocalPoint HorseRouteLots(const FRouteLotsRequest &Request) {
  return LevelLayoutAdapters::FromPostOfficeLots(
      {Request.Geometry, Request.EastLots, Request.NorthLots,
       LevelLayoutAdapters::RoadSurfaceClearance(Request.Geometry)});
}

/** User Story: As a horses route seed consumer, I need to invoke patrol route from fields through a stable signature so the horses route seed workflow remains explicit and composable. @fn TArray<FLevelLocalPoint> PatrolRouteFromFields(const FPatrolRouteFieldsRequest &Request) */
TArray<FLevelLocalPoint>
PatrolRouteFromFields(const FPatrolRouteFieldsRequest &Request) {
  const ForbocAI::Game::Data::FGeometrySettings Geometry =
      Request.Geometry;
  const FRouteLotsProjector ProjectLots = Request.ProjectLots;
  return func::map_array<FHorseRoutePointSource, FLevelLocalPoint>(
      JsonAdapters::MapSettingsJsonValues<FHorseRoutePointSource>(
          Request.Points, JSON_SETTINGS_ATOMS(EastLots, NorthLots)),
      [Geometry, ProjectLots](const FHorseRoutePointSource &Fields) {
        return ProjectLots({Geometry, Fields.EastLots, Fields.NorthLots});
      });
}

/** User Story: As a horses route seed consumer, I need to invoke horse route from fields through a stable signature so the horses route seed workflow remains explicit and composable. @fn FHorseRouteSeed HorseRouteFromFields( const FHorseRouteBuildRequest &Request) */
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

/** User Story: As a horses route seed consumer, I need to invoke build horse route seed through a stable signature so the horses route seed workflow remains explicit and composable. @fn TArray<FHorseRouteSeed> BuildHorseRouteSeed( const FBotSeedBuildRequest &Request) */
TArray<FHorseRouteSeed> BuildHorseRouteSeed(
    const FBotSeedBuildRequest &Request) {
  return func::map_array<FHorseRouteSource, FHorseRouteSeed>(
      JsonAdapters::MapSettingsJsonValues<FHorseRouteSource>(
          JsonAdapters::LoadRequiredArrayFromContent(
              {Request.RelativeJsonPath}),
          JSON_SETTINGS_ATOMS(Id, Name, bMountedRider, PatrolRoute)),
      [&Request](const FHorseRouteSource &Fields) {
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
  /** User Story: As a horses route seed consumer, I need to invoke fields through a stable signature so the horses route seed workflow remains explicit and composable. @fn static const TArray<TComponentSourceValueFieldDeclaration<FHorseRouteSeed>> &Fields() */
  static const TArray<TComponentSourceValueFieldDeclaration<FHorseRouteSeed>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FHorseRouteSeed>>
        SourceFields = ComponentSourceFieldDeclarations<FHorseRouteSeed>(
            {{"Id", &FHorseRouteSeed::Id},
             {"Name", &FHorseRouteSeed::Name},
             {"MountedRider", &FHorseRouteSeed::bMountedRider},
             {"PatrolRoute", &FHorseRouteSeed::PatrolRoute}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FHorseRouteSeed> {
  /** User Story: As a horses route seed consumer, I need to invoke the callable value through a stable signature so the horses route seed workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FHorseRouteSeed &Horse) const */
  ecs::FComponentValue operator()(const FHorseRouteSeed &Horse) const {
    return ComponentSourceValueMap(
        Horse, {"Id", "Name", "MountedRider", "PatrolRoute"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a horses route seed consumer, I need to invoke bot entity key through a stable signature so the horses route seed workflow remains explicit and composable. @fn extern ecs::EntityKey BotEntityKey(const FString &Id) */
extern ecs::EntityKey BotEntityKey(const FString &Id);

/** User Story: As a horses route seed consumer, I need to invoke project horse through a stable signature so the horses route seed workflow remains explicit and composable. @fn ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) */
ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
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
