#include "Features/Entities/Environments/Landmarks/Adapters.h"

#include "Features/Components/Data/Json/Settings/Adapters.h"
#include "Features/Components/Spatial/Level/Layout/Adapters.h"
#include "Features/Systems/Landmarks/Slice.h"
#include "Features/Components/Adapters.h"
#include "Features/Entities/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarksAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

struct FLandmarkFields {
  FString Id;
  FString Label;
  FString Kind;
  float EastLots;
  float NorthLots;
  float YawDegrees;
  float FrontageFeet;
  float DepthFeet;
  float Stories;
};

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Game::Level;
namespace LandmarkTypes = ForbocAI::Game::Level::LandmarksAdapters;

using ELandmarkKind = LevelTypes::ELandmarkKind;
using FLandmarkFields = LandmarkTypes::FLandmarkFields;

template <> struct TJsonTextValueRegistry<ELandmarkKind> {
  static const TArray<TTextValueDeclaration<ELandmarkKind>> &Values() {
    static const TArray<TTextValueDeclaration<ELandmarkKind>>
        RegisteredValues = {{"building", ELandmarkKind::Building},
                            {"road", ELandmarkKind::Road},
                            {"creek", ELandmarkKind::Creek},
                            {"terrain_marker", ELandmarkKind::TerrainMarker},
                            {"mine", ELandmarkKind::Mine},
                            {"cemetery", ELandmarkKind::Cemetery},
                            {"park", ELandmarkKind::Park}};
    return RegisteredValues;
  }
};

JSON_SETTINGS_REGISTRY(FLandmarkFields, Id, Label, Kind, EastLots, NorthLots,
                       YawDegrees, FrontageFeet, DepthFeet, Stories);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarksAdapters {
namespace {

struct FLandmarkBuildRequest {
  const FLevelTerrainData *TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FLandmarkFields Fields;
};

ELandmarkKind LandmarkKindFromJson(const FString &Kind) {
  return JsonAdapters::RequireRegisteredTextValue<ELandmarkKind>(
      Kind, TEXT("landmark kind"));
}

FLandmark LandmarkFromFields(const FLandmarkBuildRequest &Request) {
  const FVector Scale = LevelLayoutAdapters::BuildingScaleFromFeet(
      {Request.Geometry, Request.Fields.FrontageFeet,
       Request.Fields.DepthFeet, Request.Fields.Stories});
  const FLevelLocalPoint Local = LevelLayoutAdapters::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutAdapters::FromPostOfficeLots(
           {Request.Geometry, Request.Fields.EastLots,
            Request.Fields.NorthLots, 0.0f}),
       Scale, LevelLayoutAdapters::BuildingFoundationHeight(Request.Geometry)});

  return LandmarkFactories::Landmark(
      {Request.Fields.Id, Request.Fields.Label,
       LandmarkKindFromJson(Request.Fields.Kind),
       LevelLayoutAdapters::ToWorld({*Request.TerrainData, Local}),
       FRotator(0.0f, Request.Fields.YawDegrees, 0.0f), Scale});
}

} // namespace

TArray<FLandmark>
BuildLandmarkSeed(const FLandmarkSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FLandmarkFields, FLandmark>(
      JsonAdapters::ReadSettingsObjectArrayField<FLandmarkFields>(
          "Landmarks",
          JSON_SETTINGS_ATOMS(Id, Label, Kind, EastLots, NorthLots,
                              YawDegrees, FrontageFeet, DepthFeet, Stories))(Root),
      [&Request](const FLandmarkFields &Fields) {
        return LandmarkFromFields(
            {&Request.TerrainData, Request.Geometry, Fields});
      });
}

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {

namespace {

TArray<TArray<FString>> LandmarkDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Entities", "Environments", "Landmarks"},
       {"Systems", "Landmarks"}});
}

} // namespace

namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<ELandmarkKind> {
  static const TArray<TComponentTextDeclaration<ELandmarkKind>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<ELandmarkKind>>
        RegisteredCases = {{ELandmarkKind::Building, "Building"},
                           {ELandmarkKind::Road, "Road"},
                           {ELandmarkKind::Creek, "Creek"},
                           {ELandmarkKind::TerrainMarker, "TerrainMarker"},
                           {ELandmarkKind::Mine, "Mine"},
                           {ELandmarkKind::Cemetery, "Cemetery"},
                           {ELandmarkKind::Park, "Park"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FLandmark> {
  static const TArray<TComponentSourceValueFieldDeclaration<FLandmark>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FLandmark>>
        RegisteredFields = {{"Id", &FLandmark::Id},
                            {"Label", &FLandmark::Label},
                            {"Kind", &FLandmark::Kind},
                            {"Location", &FLandmark::Location},
                            {"Scale", &FLandmark::Scale}};
    return RegisteredFields;
  }
};

template <>
struct TComponentSourceProjector<FLandmark> {
  ecs::FComponentValue operator()(const FLandmark &Landmark) const {
    return ComponentSourceValueMap(
        Landmark, {"Id", "Label", "Kind", "Location", "Scale"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::EntityKey LandmarkEntityKey(const FString &Id) {
  return FString::Printf(TEXT("landmark:%s"), *Id);
}

ecs::FWorld ProjectLandmark(const FProjectLandmarkEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectLandmarkEntityPayload &PayloadValue) {
            return LandmarkEntityKey(PayloadValue.Landmark.Id);
          },
          func::constant<TArray<TArray<FString>>>(LandmarkDomains()),
          [](const FProjectLandmarkEntityPayload &PayloadValue)
              -> const FLandmark & {
            return PayloadValue.Landmark;
          },
          RegisteredComponentGroups<FLandmark>(
              {{"Components/Data", {"Id", "Label", "Kind"}},
               {"Components/Spatial", {"Location", "Scale"}}})});
}

} // namespace EntitiesAdapters

} // namespace Level
} // namespace Game
} // namespace ForbocAI
