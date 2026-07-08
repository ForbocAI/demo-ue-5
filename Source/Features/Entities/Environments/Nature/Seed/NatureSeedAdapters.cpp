#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

enum class ENatureScaleMode { LongFeature, Pad };

struct FNatureScaleModeSource {
  FString Mode;
};

struct FNaturePadScaleSource {
  float WidthFeet;
  float DepthFeet;
  float HeightFeet;
};

struct FNatureLongFeatureScaleSource {
  float WidthFeet;
  float LengthLots;
  float HeightFeet;
};

struct FNatureFeatureSource {
  FString Id;
  FString Name;
  FString Kind;
  float EastLots;
  float NorthLots;
  TSharedPtr<FJsonObject> Scale;
};

struct FFeatureLotsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
  FVector Scale;
};

struct FScaleFieldsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  TSharedPtr<FJsonObject> Object;
};

typedef FVector (*FScaleFieldsProjector)(const FScaleFieldsRequest &);

struct FScaleProjectorDeclaration {
  ENatureScaleMode Mode;
  FScaleFieldsProjector Project;

  FScaleProjectorDeclaration() = default;

  FScaleProjectorDeclaration(ENatureScaleMode InMode,
                             FScaleFieldsProjector InProject)
      : Mode(InMode), Project(InProject) {}
};

struct FFeatureBuildRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FNatureFeatureSource Fields;
};

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Game::Level;
namespace NatureTypes = ForbocAI::Game::Level::NatureAdapters;

using EFeatureKind = LevelTypes::EFeatureKind;
using ENatureScaleMode = NatureTypes::ENatureScaleMode;
using FNatureFeatureSource = NatureTypes::FNatureFeatureSource;
using FNatureLongFeatureScaleSource =
    NatureTypes::FNatureLongFeatureScaleSource;
using FNaturePadScaleSource = NatureTypes::FNaturePadScaleSource;
using FNatureScaleModeSource = NatureTypes::FNatureScaleModeSource;

template <>
struct TJsonTextValueRegistry<EFeatureKind> {
  static const TArray<TTextValueDeclaration<EFeatureKind>> &Values() {
    static const TArray<TTextValueDeclaration<EFeatureKind>>
        RegisteredValues = {{"water", EFeatureKind::Water},
                            {"rock", EFeatureKind::Rock},
                            {"tree_grove", EFeatureKind::TreeGrove},
                            {"shrub", EFeatureKind::Shrub},
                            {"water_system_marker",
                             EFeatureKind::WaterSystemMarker},
                            {"pcg_marker", EFeatureKind::PCGMarker}};
    return RegisteredValues;
  }
};

template <>
struct TJsonTextValueRegistry<ENatureScaleMode> {
  static const TArray<TTextValueDeclaration<ENatureScaleMode>> &Values() {
    static const TArray<TTextValueDeclaration<ENatureScaleMode>>
        RegisteredValues = {{"long_feature", ENatureScaleMode::LongFeature},
                            {"pad", ENatureScaleMode::Pad},
                            {"pad_feet", ENatureScaleMode::Pad}};
    return RegisteredValues;
  }
};

JSON_SETTINGS_REGISTRY(FNatureScaleModeSource, Mode);
JSON_SETTINGS_REGISTRY(FNaturePadScaleSource, WidthFeet, DepthFeet, HeightFeet);
JSON_SETTINGS_REGISTRY(FNatureLongFeatureScaleSource, WidthFeet, LengthLots,
                       HeightFeet);
JSON_SETTINGS_REGISTRY(FNatureFeatureSource, Id, Name, Kind, EastLots,
                       NorthLots, Scale);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {
namespace {

FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) {
  return LevelLayoutAdapters::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutAdapters::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
      Request.Scale,
      LevelLayoutAdapters::RoadSurfaceClearance(Request.Geometry)});
}

EFeatureKind NatureKindFromJson(const FString &Kind) {
  return JsonAdapters::RequireRegisteredTextValue<EFeatureKind>(
      Kind, TEXT("nature feature kind"));
}

ENatureScaleMode NatureScaleModeFromJson(const FString &Mode) {
  return JsonAdapters::RequireRegisteredTextValue<ENatureScaleMode>(
      Mode, TEXT("nature feature scale mode"));
}

FVector PadScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNaturePadScaleSource Fields =
      JsonAdapters::ReadSettingsFields<FNaturePadScaleSource>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, DepthFeet,
                                              HeightFeet));
  return LevelLayoutAdapters::PadScaleFromFeet(
      {Request.Geometry, Fields.WidthFeet, Fields.DepthFeet,
       Fields.HeightFeet});
}

FVector LongFeatureScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNatureLongFeatureScaleSource Fields =
      JsonAdapters::ReadSettingsFields<FNatureLongFeatureScaleSource>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, LengthLots,
                                              HeightFeet));
  return LevelLayoutAdapters::LongFeatureScale(
      {Request.Geometry, Fields.WidthFeet, Fields.LengthLots,
       Fields.HeightFeet});
}

const TArray<FScaleProjectorDeclaration> &ScaleProjectorDeclarations() {
  static const TArray<FScaleProjectorDeclaration> Declarations = {
      {ENatureScaleMode::LongFeature, LongFeatureScaleFromFields},
      {ENatureScaleMode::Pad, PadScaleFromFields}};
  return Declarations;
}

func::Maybe<FScaleProjectorDeclaration>
FindScaleProjector(ENatureScaleMode Mode) {
  return func::find_array<FScaleProjectorDeclaration>(
      ScaleProjectorDeclarations(),
      [Mode](const FScaleProjectorDeclaration &Declaration) {
        return Declaration.Mode == Mode;
      });
}

FVector ScaleObjectFromFields(const FScaleFieldsRequest &Request) {
  const FNatureScaleModeSource Fields =
      JsonAdapters::ReadSettingsFields<FNatureScaleModeSource>(
          Request.Object, JSON_SETTINGS_ATOMS(Mode));
  const func::Maybe<FScaleProjectorDeclaration> Projector =
      FindScaleProjector(NatureScaleModeFromJson(Fields.Mode));
  checkf(Projector.hasValue, TEXT("Invalid nature feature scale mode"));
  return Projector.value.Project(Request);
}

FVector ScaleFromFields(const FFeatureBuildRequest &Request) {
  return ScaleObjectFromFields({Request.Geometry, Request.Fields.Scale});
}

FFeatureSeed FeatureFromFields(const FFeatureBuildRequest &Request) {
  const FVector Scale = ScaleFromFields(Request);
  FFeatureSeed Seed;
  Seed.Id = Request.Fields.Id;
  Seed.Name = Request.Fields.Name;
  Seed.Kind = NatureKindFromJson(Request.Fields.Kind);
  Seed.Scale = Scale;
  Seed.Location = FeatureLots(
      {Request.Geometry, Request.Fields.EastLots, Request.Fields.NorthLots,
       Scale});
  return Seed;
}

} // namespace

TArray<FFeatureSeed> BuildNatureSeed(
    const FBuildRequest &Request) {
  return func::map_array<FNatureFeatureSource, FFeatureSeed>(
      JsonAdapters::MapJsonValues<FNatureFeatureSource>(
          JsonAdapters::LoadRequiredArrayFromContent(
              {Request.RelativeJsonPath}),
          JsonAdapters::ReadSettingsWith<FNatureFeatureSource>(
              JSON_SETTINGS_ATOMS(Id, Name, Kind, EastLots, NorthLots,
                                  Scale))),
      [&Request](const FNatureFeatureSource &Fields) {
        return FeatureFromFields({Request.Geometry, Fields});
      });
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EFeatureKind> {
  static const TArray<TComponentTextDeclaration<EFeatureKind>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EFeatureKind>>
        RegisteredCases = {{EFeatureKind::Water, "Water"},
                           {EFeatureKind::Rock, "Rock"},
                           {EFeatureKind::TreeGrove, "TreeGrove"},
                           {EFeatureKind::Shrub, "Shrub"},
                           {EFeatureKind::PCGMarker, "PCGMarker"},
                           {EFeatureKind::WaterSystemMarker,
                            "WaterSystemMarker"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FFeatureSeed> {
  static const TArray<
      TComponentSourceValueFieldDeclaration<FFeatureSeed>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FFeatureSeed>>
        SourceFields = ComponentSourceFieldDeclarations<FFeatureSeed>(
            {{"Id", &FFeatureSeed::Id},
             {"Name", &FFeatureSeed::Name},
             {"Kind", &FFeatureSeed::Kind},
             {"LocalLocation", &FFeatureSeed::Location},
             {"Scale", &FFeatureSeed::Scale}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FFeatureSeed> {
  ecs::FComponentValue
  operator()(const FFeatureSeed &NatureFeature) const {
    return ComponentSourceValueMap(
        NatureFeature, {"Id", "Name", "Kind", "LocalLocation", "Scale"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::EntityKey NatureEntityKey(const FString &Id) {
  return FString::Printf(TEXT("nature:%s"), *Id);
}

ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectNatureFeatureEntityPayload &PayloadValue) {
            return NatureEntityKey(PayloadValue.Feature.Id);
          },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Environments", "Nature"},
                   {"Systems", "Nature"}})),
          [](const FProjectNatureFeatureEntityPayload &PayloadValue)
              -> const FFeatureSeed & {
            return PayloadValue.Feature;
          },
          RegisteredComponentGroups<FFeatureSeed>(
              {{"Components/Data", {"Id", "Name", "Kind"}},
               {"Components/Spatial", {"LocalLocation", "Scale"}}})});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
