#include "Features/Entities/Environments/Landmarks/EnvironmentsLandmarksAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSlice.h"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarksAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

struct FIdentitySource {
  FString Id;
  FString Label;
  FString Kind;
};

struct FLotSource {
  float EastLots;
  float NorthLots;
  float YawDegrees;
};

struct FShapeSource {
  float FrontageFeet;
  float DepthFeet;
  float Stories;
};

struct FLandmarkSource {
  FIdentitySource Identity;
  FLotSource Lot;
  FShapeSource Shape;
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
using FLandmarkSource = LandmarkTypes::FLandmarkSource;

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

template <> struct TJsonSettingsRegistry<FLandmarkSource> {
  static const TArray<TField<FLandmarkSource>> &Fields() {
    static const TArray<TField<FLandmarkSource>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(Id),
                NestedFieldMembers(&FLandmarkSource::Identity,
                                   &LandmarkTypes::FIdentitySource::Id)),
            NestedSettingField(
                JSON_SETTING_ATOM(Label),
                NestedFieldMembers(&FLandmarkSource::Identity,
                                   &LandmarkTypes::FIdentitySource::Label)),
            NestedSettingField(
                JSON_SETTING_ATOM(Kind),
                NestedFieldMembers(&FLandmarkSource::Identity,
                                   &LandmarkTypes::FIdentitySource::Kind)),
            NestedSettingField(
                JSON_SETTING_ATOM(EastLots),
                NestedFieldMembers(&FLandmarkSource::Lot,
                                   &LandmarkTypes::FLotSource::EastLots)),
            NestedSettingField(
                JSON_SETTING_ATOM(NorthLots),
                NestedFieldMembers(&FLandmarkSource::Lot,
                                   &LandmarkTypes::FLotSource::NorthLots)),
            NestedSettingField(
                JSON_SETTING_ATOM(YawDegrees),
                NestedFieldMembers(&FLandmarkSource::Lot,
                                   &LandmarkTypes::FLotSource::YawDegrees)),
            NestedSettingField(
                JSON_SETTING_ATOM(FrontageFeet),
                NestedFieldMembers(&FLandmarkSource::Shape,
                                   &LandmarkTypes::FShapeSource::FrontageFeet)),
            NestedSettingField(
                JSON_SETTING_ATOM(DepthFeet),
                NestedFieldMembers(&FLandmarkSource::Shape,
                                   &LandmarkTypes::FShapeSource::DepthFeet)),
            NestedSettingField(
                JSON_SETTING_ATOM(Stories),
                NestedFieldMembers(&FLandmarkSource::Shape,
                                   &LandmarkTypes::FShapeSource::Stories))};
    return RegisteredFields;
  }
};

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
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FLandmarkSource Fields;
};

ELandmarkKind LandmarkKindFromJson(const FString &Kind) {
  return JsonAdapters::RequireRegisteredTextValue<ELandmarkKind>(
      Kind, TEXT("landmark kind"));
}

FLandmark LandmarkFromFields(const FLandmarkBuildRequest &Request) {
  const FVector Scale = LevelLayoutAdapters::BuildingScaleFromFeet(
      {Request.Geometry, Request.Fields.Shape.FrontageFeet,
       Request.Fields.Shape.DepthFeet, Request.Fields.Shape.Stories});
  const FLevelLocalPoint Local = LevelLayoutAdapters::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutAdapters::FromPostOfficeLots(
           {Request.Geometry, Request.Fields.Lot.EastLots,
            Request.Fields.Lot.NorthLots, 0.0f}),
       Scale, LevelLayoutAdapters::BuildingFoundationHeight(Request.Geometry)});

  return LandmarkFactories::Landmark(
      {Request.Fields.Identity.Id, Request.Fields.Identity.Label,
       LandmarkKindFromJson(Request.Fields.Identity.Kind),
       LevelLayoutAdapters::ToWorld({*Request.TerrainData, Local}),
       FRotator(0.0f, Request.Fields.Lot.YawDegrees, 0.0f), Scale});
}

} // namespace

TArray<FLandmark>
BuildLandmarkSeed(const FLandmarkSeedBuildRequest &Request) {
  return func::map_array<FLandmarkSource, FLandmark>(
      JsonAdapters::MapSettingsJsonValues<FLandmarkSource>(
          JsonAdapters::LoadRequiredArrayFromContent(
              {Request.RelativeJsonPath}),
          JSON_SETTINGS_ATOMS(Id, Label, Kind, EastLots, NorthLots,
                              YawDegrees, FrontageFeet, DepthFeet, Stories)),
      [&Request](const FLandmarkSource &Fields) {
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
        SourceFields = ComponentSourceFieldDeclarations<FLandmark>(
            {{"Id", &FLandmark::Id},
             {"Label", &FLandmark::Label},
             {"Kind", &FLandmark::Kind},
             {"Location", &FLandmark::Location},
             {"Scale", &FLandmark::Scale}});
    return SourceFields;
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
  return ComponentsAdapters::ProjectEntityCatalog(
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
