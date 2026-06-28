#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

namespace {

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
  const JsonAdapters::FJsonFloatReader Float =
      JsonAdapters::FloatIn(Request.Scale);
  return LevelLayoutSlice::PadScaleFromFeet(
      {Request.Geometry, Float(TEXT("width_feet")), Float(TEXT("depth_feet")),
       Float(TEXT("height_feet"))});
}

FVector LongFeatureScaleFromJson(const FScaleFromJsonRequest &Request) {
  const JsonAdapters::FJsonFloatReader Float =
      JsonAdapters::FloatIn(Request.Scale);
  return LevelLayoutSlice::LongFeatureScale(
      {Request.Geometry, Float(TEXT("width_feet")), Float(TEXT("length_lots")),
       Float(TEXT("height_feet"))});
}

FVector ScaleObjectFromJson(const FScaleFromJsonRequest &Request) {
  const JsonAdapters::FJsonStringReader String =
      JsonAdapters::StringIn(Request.Scale);
  const func::Maybe<FVector> Parsed =
      func::multi_match<FString, FVector>(
          String(TEXT("mode")),
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
  const JsonAdapters::FJsonObjectReader Object =
      JsonAdapters::ObjectIn(Request.FeatureObject);
  const func::Maybe<TSharedPtr<FJsonObject>> Scale =
      Object(TEXT("scale"));
  checkf(Scale.hasValue, TEXT("Nature feature scale object is required"));
  return ScaleObjectFromJson({Request.Geometry, Scale.value});
}

FNatureFeatureSeed FeatureFromJson(const FFeatureFromJsonRequest &Request) {
  const JsonAdapters::FJsonStringReader String =
      JsonAdapters::StringIn(Request.FeatureObject);
  const JsonAdapters::FJsonFloatReader Float =
      JsonAdapters::FloatIn(Request.FeatureObject);
  FNatureFeatureSeed Seed;
  Seed.Id = String(TEXT("id"));
  Seed.Name = String(TEXT("name"));
  Seed.Kind = NatureKindFromJson(String(TEXT("kind")));
  Seed.Scale = ScaleFromJson(Request);
  Seed.Location = FeatureLots(
      {Request.Geometry, Float(TEXT("east_lots")), Float(TEXT("north_lots")),
       Seed.Scale});
  return Seed;
}

} // namespace

TArray<FNatureFeatureSeed> BuildNatureSeed(
    const FNatureSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  const JsonAdapters::FJsonArrayReader Array = JsonAdapters::ArrayIn(Root);
  return JsonAdapters::MapJsonValues<FNatureFeatureSeed>(
      Array(TEXT("features")),
      [&Request](const TSharedPtr<FJsonObject> &FeatureObject) {
        return FeatureFromJson({Request.Geometry, FeatureObject});
      });
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
