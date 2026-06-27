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
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
  FVector Scale = FVector::OneVector;
};

FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) {
  return LevelLayoutSlice::CenteredOnGround(
      LevelLayoutSlice::FromPostOfficeLots(Request.EastLots, Request.NorthLots),
      Request.Scale,
      LevelLayoutSlice::RoadSurfaceClearance());
}

ENatureFeatureKind NatureKindFromJson(const FString &Kind) {
  return func::or_else(
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
          }),
      ENatureFeatureKind::PCGMarker);
}

FVector PadScaleFromJson(const TSharedPtr<FJsonObject> &Scale) {
  return LevelLayoutSlice::PadScaleFromFeet(
      DataAdapters::ReadFloat({Scale, TEXT("width_feet")}),
      DataAdapters::ReadFloat({Scale, TEXT("depth_feet")}),
      DataAdapters::ReadFloat({Scale, TEXT("height_feet")}));
}

FVector LongFeatureScaleFromJson(const TSharedPtr<FJsonObject> &Scale) {
  return LevelLayoutSlice::LongFeatureScale(
      DataAdapters::ReadFloat({Scale, TEXT("width_feet")}),
      DataAdapters::ReadFloat({Scale, TEXT("length_lots")}),
      DataAdapters::ReadFloat({Scale, TEXT("height_feet")}));
}

FVector ScaleObjectFromJson(const TSharedPtr<FJsonObject> &Scale) {
  return func::or_else(
      func::multi_match<FString, FVector>(
          DataAdapters::ReadString({Scale, TEXT("mode")}),
          {func::when<FString, FVector>(
              func::equals<FString>(TEXT("long_feature")),
              [Scale](const FString &) {
                return LongFeatureScaleFromJson(Scale);
              })}),
      PadScaleFromJson(Scale));
}

FVector ScaleFromJson(const TSharedPtr<FJsonObject> &FeatureObject) {
  return func::match(
      DataAdapters::ReadObject({FeatureObject, TEXT("scale")}),
      [](const TSharedPtr<FJsonObject> &Scale) {
        return ScaleObjectFromJson(Scale);
      },
      []() { return FVector::OneVector; });
}

FNatureFeatureSeed FeatureFromJson(
    const TSharedPtr<FJsonObject> &FeatureObject) {
  FNatureFeatureSeed Seed;
  Seed.Id = DataAdapters::ReadString({FeatureObject, TEXT("id")});
  Seed.Name = DataAdapters::ReadString({FeatureObject, TEXT("name")});
  Seed.Kind = NatureKindFromJson(
      DataAdapters::ReadString({FeatureObject, TEXT("kind")}));
  Seed.Scale = ScaleFromJson(FeatureObject);
  Seed.Location = FeatureLots(
      {DataAdapters::ReadFloat({FeatureObject, TEXT("east_lots")}),
       DataAdapters::ReadFloat({FeatureObject, TEXT("north_lots")}),
       Seed.Scale});
  return Seed;
}

} // namespace

TArray<FNatureFeatureSeed> BuildClearCreekNatureSeed() {
  return func::match(
      DataAdapters::LoadObjectFromContent({NatureDataPath}),
      [](const TSharedPtr<FJsonObject> &Root) {
        return DataAdapters::MapJsonValues<FNatureFeatureSeed>(
            {DataAdapters::ReadArray({Root, TEXT("features")}),
             FeatureFromJson});
      },
      []() { return TArray<FNatureFeatureSeed>(); });
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
