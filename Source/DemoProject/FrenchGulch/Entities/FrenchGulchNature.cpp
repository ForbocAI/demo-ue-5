#include "FrenchGulch/Entities/FrenchGulchNature.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {
namespace FrenchGulchNature {
namespace {
FFrenchGulchLocalPoint FeatureLots(float EastLots, float NorthLots,
                                   const FVector &Scale) {
  return FrenchGulchLayout::CenteredOnGround(
      FrenchGulchLayout::FromPostOfficeLots(EastLots, NorthLots), Scale,
      FrenchGulchLayout::RoadSurfaceClearance());
}

FFrenchGulchNatureFeatureSeed
Feature(const TCHAR *Name, EFrenchGulchNatureFeatureKind Kind, float EastLots,
        float NorthLots, const FVector &Scale) {
  FFrenchGulchNatureFeatureSeed Seed;
  Seed.Name = FString(Name);
  Seed.Kind = Kind;
  Seed.Scale = Scale;
  Seed.Location = FeatureLots(EastLots, NorthLots, Scale);
  return Seed;
}

FVector WaterRibbonScale(float WidthFeet, float LengthLots) {
  return FrenchGulchLayout::LongFeatureScale(WidthFeet, LengthLots, 0.75f);
}

FVector RockScale(float WidthFeet, float DepthFeet, float HeightFeet) {
  return FrenchGulchLayout::PadScaleFromFeet(WidthFeet, DepthFeet, HeightFeet);
}

FVector GroveScale(float WidthFeet, float DepthFeet, float HeightFeet) {
  return FrenchGulchLayout::PadScaleFromFeet(WidthFeet, DepthFeet, HeightFeet);
}
} // namespace

TArray<FFrenchGulchNatureFeatureSeed> BuildClearCreekNatureSeed() {
  const FVector CreekWaterScale = WaterRibbonScale(8.0f, 22.5f);
  const FVector CreekShallowScale = WaterRibbonScale(5.0f, 9.0f);
  const FVector BoulderLargeScale = RockScale(10.0f, 8.0f, 4.5f);
  const FVector BoulderSmallScale = RockScale(6.0f, 5.0f, 3.0f);
  const FVector TreeGroveScale = GroveScale(25.0f, 34.0f, 18.0f);
  const FVector ShrubScale = GroveScale(18.0f, 15.0f, 5.5f);
  const FVector MarkerScale = GroveScale(12.0f, 12.0f, 7.0f);

  return {
      Feature(TEXT("Water System Clear Creek spline target"),
              EFrenchGulchNatureFeatureKind::WaterSystemMarker, 4.8f, 0.0f,
              MarkerScale),
      Feature(TEXT("Clear Creek water ribbon"),
              EFrenchGulchNatureFeatureKind::Water, 4.85f, 0.0f,
              CreekWaterScale),
      Feature(TEXT("Clear Creek shallow bend"),
              EFrenchGulchNatureFeatureKind::Water, 4.35f, -3.7f,
              CreekShallowScale),
      Feature(TEXT("County Park boulder run"),
              EFrenchGulchNatureFeatureKind::Rock, 4.0f, 4.9f,
              BoulderLargeScale),
      Feature(TEXT("South creek boulder cluster"),
              EFrenchGulchNatureFeatureKind::Rock, 5.25f, -3.4f,
              BoulderSmallScale),
      Feature(TEXT("Riparian cottonwood grove"),
              EFrenchGulchNatureFeatureKind::TreeGrove, 5.35f, 1.8f,
              TreeGroveScale),
      Feature(TEXT("County Park shade grove"),
              EFrenchGulchNatureFeatureKind::TreeGrove, 3.95f, 5.45f,
              TreeGroveScale),
      Feature(TEXT("Creek bank willow shrubs"),
              EFrenchGulchNatureFeatureKind::Shrub, 5.75f, -1.2f,
              ShrubScale),
      Feature(TEXT("Roadside scrub and rock scatter"),
              EFrenchGulchNatureFeatureKind::Shrub, 1.25f, -4.2f,
              ShrubScale),
      Feature(TEXT("PCG scatter volume target"),
              EFrenchGulchNatureFeatureKind::PCGMarker, 6.15f, 2.85f,
              MarkerScale),
  };
}

} // namespace FrenchGulchNature
} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
