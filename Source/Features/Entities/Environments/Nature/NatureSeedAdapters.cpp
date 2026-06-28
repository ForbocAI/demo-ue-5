#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"

#include "Features/Components/Data/DataAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureAdapters {

namespace DataAdapters = ForbocAI::Demo::Data::DataAdapters;

namespace {

constexpr const TCHAR *NatureDataPath = TEXT("Data/french_gulch_nature.json");

struct FFeatureLotsRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
  FVector Scale = FVector::OneVector;
};

struct FScaleFromJsonRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  TSharedPtr<FJsonObject> Scale;
};

struct FFeatureFromJsonRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  TSharedPtr<FJsonObject> FeatureObject;
};

FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) {
  return LevelLayoutSlice::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutSlice::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
      Request.Scale,
      LevelLayoutSlice::RoadSurfaceClearance(Request.Geometry)});
}

ENatureFeatureKind NatureKindFromJson(const FString &Kind) {
  const func::Maybe<ENatureFeatureKind> Parsed =
      func::multi_match<FString, ENatureFeatureKind>(
          Kind.ToLower(),
          {
              func::when<FString, ENatureFeatureKind>(
                  func::equals<FString>(TEXT("water")),
                  [](const FString &) { return ENatureFeatureKind::Water; }),
              func::when<FString, ENatureFeatureKind>(
                  func::equals<FString>(TEXT("rock")),
                  [](const FString &) { return ENatureFeatureKind::Rock; }),
              func::when<FString, ENatureFeatureKind>(
                  func::equals<FString>(TEXT("tree_grove")),
                  [](const FString &) {
                    return ENatureFeatureKind::TreeGrove;
                  }),
              func::when<FString, ENatureFeatureKind>(
                  func::equals<FString>(TEXT("shrub")),
                  [](const FString &) { return ENatureFeatureKind::Shrub; }),
              func::when<FString, ENatureFeatureKind>(
                  func::equals<FString>(TEXT("water_system_marker")),
                  [](const FString &) {
                    return ENatureFeatureKind::WaterSystemMarker;
                  }),
              func::when<FString, ENatureFeatureKind>(
                  func::equals<FString>(TEXT("pcg_marker")),
                  [](const FString &) {
                    return ENatureFeatureKind::PCGMarker;
                  }),
          });
  checkf(Parsed.hasValue, TEXT("Invalid nature feature kind: %s"), *Kind);
  return Parsed.value;
}

FVector PadScaleFromJson(const FScaleFromJsonRequest &Request) {
  return LevelLayoutSlice::PadScaleFromFeet(
      {Request.Geometry,
       DataAdapters::ReadFloat({Request.Scale, TEXT("width_feet")}),
       DataAdapters::ReadFloat({Request.Scale, TEXT("depth_feet")}),
       DataAdapters::ReadFloat({Request.Scale, TEXT("height_feet")})});
}

FVector LongFeatureScaleFromJson(const FScaleFromJsonRequest &Request) {
  return LevelLayoutSlice::LongFeatureScale(
      {Request.Geometry,
       DataAdapters::ReadFloat({Request.Scale, TEXT("width_feet")}),
       DataAdapters::ReadFloat({Request.Scale, TEXT("length_lots")}),
       DataAdapters::ReadFloat({Request.Scale, TEXT("height_feet")})});
}

FVector ScaleObjectFromJson(const FScaleFromJsonRequest &Request) {
  const func::Maybe<FVector> Parsed =
      func::multi_match<FString, FVector>(
          DataAdapters::ReadString({Request.Scale, TEXT("mode")}),
          {
              func::when<FString, FVector>(
                  func::equals<FString>(TEXT("long_feature")),
                  [&Request](const FString &) {
                    return LongFeatureScaleFromJson(Request);
                  }),
              func::when<FString, FVector>(
                  func::equals<FString>(TEXT("pad")),
                  [&Request](const FString &) {
                    return PadScaleFromJson(Request);
                  }),
              func::when<FString, FVector>(
                  func::equals<FString>(TEXT("pad_feet")),
                  [&Request](const FString &) {
                    return PadScaleFromJson(Request);
                  }),
          });
  checkf(Parsed.hasValue, TEXT("Invalid nature feature scale mode"));
  return Parsed.value;
}

FVector ScaleFromJson(const FFeatureFromJsonRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Scale =
      DataAdapters::ReadObject({Request.FeatureObject, TEXT("scale")});
  checkf(Scale.hasValue, TEXT("Nature feature scale object is required"));
  return ScaleObjectFromJson({Request.Geometry, Scale.value});
}

FNatureFeatureSeed FeatureFromJson(const FFeatureFromJsonRequest &Request) {
  FNatureFeatureSeed Seed;
  Seed.Id = DataAdapters::ReadString({Request.FeatureObject, TEXT("id")});
  Seed.Name = DataAdapters::ReadString({Request.FeatureObject, TEXT("name")});
  Seed.Kind = NatureKindFromJson(
      DataAdapters::ReadString({Request.FeatureObject, TEXT("kind")}));
  Seed.Scale = ScaleFromJson(Request);
  Seed.Location = FeatureLots(
      {Request.Geometry,
       DataAdapters::ReadFloat({Request.FeatureObject, TEXT("east_lots")}),
       DataAdapters::ReadFloat({Request.FeatureObject, TEXT("north_lots")}),
       Seed.Scale});
  return Seed;
}

} // namespace

TArray<FNatureFeatureSeed> BuildClearCreekNatureSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry) {
  return func::match(
      DataAdapters::LoadObjectFromContent({NatureDataPath}),
      [&Geometry](const TSharedPtr<FJsonObject> &Root) {
        return DataAdapters::MapJsonValues<FNatureFeatureSeed>(
            {DataAdapters::ReadArray({Root, TEXT("features")}),
             [&Geometry](const TSharedPtr<FJsonObject> &FeatureObject) {
               return FeatureFromJson({Geometry, FeatureObject});
             }});
      },
      []() { return TArray<FNatureFeatureSeed>(); });
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
