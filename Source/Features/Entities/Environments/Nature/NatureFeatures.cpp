#include "Features/Entities/NatureFeatures.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureFeatures {
namespace {
struct FFeatureSource {
  const TCHAR *Id;
  const TCHAR *Name;
  ENatureFeatureKind Kind;
  float EastLots;
  float NorthLots;
  FVector Scale;
};

FMapLocalPoint FeatureLots(float EastLots, float NorthLots,
                                   const FVector &Scale) {
  return MapLayout::CenteredOnGround(
      MapLayout::FromPostOfficeLots(EastLots, NorthLots), Scale,
      MapLayout::RoadSurfaceClearance());
}

FNatureFeatureSeed
Feature(const FFeatureSource &Source) {
  FNatureFeatureSeed Seed;
  Seed.Id = FString(Source.Id);
  Seed.Name = FString(Source.Name);
  Seed.Kind = Source.Kind;
  Seed.Scale = Source.Scale;
  Seed.Location =
      FeatureLots(Source.EastLots, Source.NorthLots, Source.Scale);
  return Seed;
}

FVector WaterRibbonScale(float WidthFeet, float LengthLots) {
  return MapLayout::LongFeatureScale(WidthFeet, LengthLots, 0.75f);
}

FVector RockScale(float WidthFeet, float DepthFeet, float HeightFeet) {
  return MapLayout::PadScaleFromFeet(WidthFeet, DepthFeet, HeightFeet);
}

FVector GroveScale(float WidthFeet, float DepthFeet, float HeightFeet) {
  return MapLayout::PadScaleFromFeet(WidthFeet, DepthFeet, HeightFeet);
}
} // namespace

TArray<FNatureFeatureSeed> BuildClearCreekNatureSeed() {
  const FVector CreekWaterScale = WaterRibbonScale(8.0f, 22.5f);
  const FVector CreekShallowScale = WaterRibbonScale(5.0f, 9.0f);
  const FVector BoulderLargeScale = RockScale(10.0f, 8.0f, 4.5f);
  const FVector BoulderSmallScale = RockScale(6.0f, 5.0f, 3.0f);
  const FVector TreeGroveScale = GroveScale(25.0f, 34.0f, 18.0f);
  const FVector ShrubScale = GroveScale(18.0f, 15.0f, 5.5f);
  const FVector DenseShrubScale = GroveScale(26.0f, 20.0f, 6.0f);
  const FVector ChaparralScale = GroveScale(32.0f, 22.0f, 7.0f);
  const FVector MarkerScale = GroveScale(12.0f, 12.0f, 7.0f);

  return {
      Feature({TEXT("water-system-clear-creek"), TEXT("Water System Clear Creek spline target"),
               ENatureFeatureKind::WaterSystemMarker, 4.8f, 0.0f,
               MarkerScale}),
      Feature({TEXT("clear-creek-water-ribbon"), TEXT("Clear Creek water ribbon"),
               ENatureFeatureKind::Water, 4.85f, 0.0f,
               CreekWaterScale}),
      Feature({TEXT("clear-creek-shallow-bend"), TEXT("Clear Creek shallow bend"),
               ENatureFeatureKind::Water, 4.35f, -3.7f,
               CreekShallowScale}),
      Feature({TEXT("county-park-boulder-run"), TEXT("County Park boulder run"),
               ENatureFeatureKind::Rock, 4.0f, 4.9f,
               BoulderLargeScale}),
      Feature({TEXT("south-creek-boulder-cluster"), TEXT("South creek boulder cluster"),
               ENatureFeatureKind::Rock, 5.25f, -3.4f,
               BoulderSmallScale}),
      Feature({TEXT("riparian-cottonwood-grove"), TEXT("Riparian cottonwood grove"),
               ENatureFeatureKind::TreeGrove, 5.35f, 1.8f,
               TreeGroveScale}),
      Feature({TEXT("county-park-shade-grove"), TEXT("County Park shade grove"),
               ENatureFeatureKind::TreeGrove, 3.95f, 5.45f,
               TreeGroveScale}),
      Feature({TEXT("creek-bank-willow-shrubs"), TEXT("Creek bank willow shrubs"),
               ENatureFeatureKind::Shrub, 5.75f, -1.2f,
               ShrubScale}),
      Feature({TEXT("clear-creek-willow-thicket-north"), TEXT("Clear Creek willow thicket north"),
               ENatureFeatureKind::Shrub, 5.55f, 3.35f,
               DenseShrubScale}),
      Feature({TEXT("clear-creek-willow-thicket-south"), TEXT("Clear Creek willow thicket south"),
               ENatureFeatureKind::Shrub, 5.65f, -4.75f,
               DenseShrubScale}),
      Feature({TEXT("hot-hillside-chaparral-east"), TEXT("Hot hillside chaparral east"),
               ENatureFeatureKind::Shrub, 7.25f, 2.0f,
               ChaparralScale}),
      Feature({TEXT("hot-hillside-chaparral-west"), TEXT("Hot hillside chaparral west"),
               ENatureFeatureKind::Shrub, -1.95f, -4.9f,
               ChaparralScale}),
      Feature({TEXT("roadside-scrub-rock-scatter"), TEXT("Roadside scrub and rock scatter"),
               ENatureFeatureKind::Shrub, 1.25f, -4.2f,
               ShrubScale}),
      Feature({TEXT("pcg-scatter-volume-target"), TEXT("PCG scatter volume target"),
               ENatureFeatureKind::PCGMarker, 6.15f, 2.85f,
               MarkerScale}),
  };
}

} // namespace NatureFeatures
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
