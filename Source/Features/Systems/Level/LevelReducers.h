#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Level/LevelTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelSystemReducers {
namespace detail {

inline FLevelLocalPoint GroundLots(float EastLots, float NorthLots,
                                   const FVector &Scale,
                                   float Clearance) {
  return LevelLayoutSlice::CenteredOnGround(
      LevelLayoutSlice::FromPostOfficeLots(EastLots, NorthLots), Scale,
      Clearance);
}

inline FLevelLocalPoint BuildingLots(float EastLots, float NorthLots,
                                     const FVector &Scale) {
  return GroundLots(EastLots, NorthLots, Scale,
                    LevelLayoutSlice::BuildingFoundationHeight());
}

inline FLevelLocalPoint FeatureLots(float EastLots, float NorthLots,
                                    const FVector &Scale) {
  return GroundLots(EastLots, NorthLots, Scale,
                    LevelLayoutSlice::RoadSurfaceClearance());
}

inline FVector FallbackTerrainScale() {
  const float PlaneScale = FLevelTerrainData::TerrainWorldSize /
                           (LevelLayoutSlice::CubeHalfExtent() * 2.0f);
  return FVector(PlaneScale, PlaneScale,
                 LevelLayoutSlice::RoadSurfaceClearance() /
                     LevelLayoutSlice::CubeHalfExtent());
}

inline FVector ScaleFromSeed(const FLevelRuntimeScaleSeed &Seed) {
  return func::or_else(
      func::multi_match<ELevelRuntimeScaleMode, FVector>(
          Seed.Mode,
          {
              func::when<ELevelRuntimeScaleMode, FVector>(
                  func::equals<ELevelRuntimeScaleMode>(
                      ELevelRuntimeScaleMode::Building),
                  [&Seed](ELevelRuntimeScaleMode) {
                    return LevelLayoutSlice::BuildingScaleFromFeet(
                        Seed.FrontageFeet, Seed.DepthFeet, Seed.Stories);
                  }),
              func::when<ELevelRuntimeScaleMode, FVector>(
                  func::equals<ELevelRuntimeScaleMode>(
                      ELevelRuntimeScaleMode::LongFeature),
                  [&Seed](ELevelRuntimeScaleMode) {
                    return LevelLayoutSlice::LongFeatureScale(
                        Seed.WidthFeet, Seed.LengthLots, Seed.HeightFeet);
                  }),
              func::when<ELevelRuntimeScaleMode, FVector>(
                  func::equals<ELevelRuntimeScaleMode>(
                      ELevelRuntimeScaleMode::FallbackTerrain),
                  [](ELevelRuntimeScaleMode) { return FallbackTerrainScale(); }),
          }),
      LevelLayoutSlice::PadScaleFromFeet(Seed.WidthFeet, Seed.DepthFeet,
                                         Seed.HeightFeet));
}

inline FLevelLocalPoint LocalPointFromSeed(
    const FLevelRuntimeBlockSeed &Seed, const FVector &Scale) {
  return func::or_else(
      func::multi_match<ELevelRuntimeAnchorMode, FLevelLocalPoint>(
          Seed.Anchor,
          {
              func::when<ELevelRuntimeAnchorMode, FLevelLocalPoint>(
                  func::equals<ELevelRuntimeAnchorMode>(
                      ELevelRuntimeAnchorMode::BuildingLots),
                  [&Seed, &Scale](ELevelRuntimeAnchorMode) {
                    return BuildingLots(Seed.EastLots, Seed.NorthLots, Scale);
                  }),
              func::when<ELevelRuntimeAnchorMode, FLevelLocalPoint>(
                  func::equals<ELevelRuntimeAnchorMode>(
                      ELevelRuntimeAnchorMode::PostOfficeLots),
                  [&Seed](ELevelRuntimeAnchorMode) {
                    return LevelLayoutSlice::FromPostOfficeLots(
                        Seed.EastLots, Seed.NorthLots);
                  }),
          }),
      FeatureLots(Seed.EastLots, Seed.NorthLots, Scale));
}

inline FVector WorldLocationFromSeed(const FLevelRuntimeBlockSeed &Seed,
                                     const FLevelTerrainData &TerrainData,
                                     const FVector &Scale) {
  return Seed.Anchor == ELevelRuntimeAnchorMode::World
             ? Seed.WorldLocation
             : LevelLayoutSlice::ToWorld(TerrainData,
                                          LocalPointFromSeed(Seed, Scale));
}

inline float LabelHeightOffsetFromSeed(const FLevelRuntimeLabelSeed &Seed,
                                       const FVector &ReferenceScale) {
  return func::or_else(
      func::multi_match<ELevelRuntimeLabelHeightMode, float>(
          Seed.Height,
          {
              func::when<ELevelRuntimeLabelHeightMode, float>(
                  func::equals<ELevelRuntimeLabelHeightMode>(
                      ELevelRuntimeLabelHeightMode::LabelForScale),
                  [&ReferenceScale](ELevelRuntimeLabelHeightMode) {
                    return LevelLayoutSlice::LabelHeightForScale(
                        ReferenceScale);
                  }),
              func::when<ELevelRuntimeLabelHeightMode, float>(
                  func::equals<ELevelRuntimeLabelHeightMode>(
                      ELevelRuntimeLabelHeightMode::AboveBlock),
                  [&ReferenceScale](ELevelRuntimeLabelHeightMode) {
                    return LevelLayoutSlice::AboveBlock(
                               LevelLayoutSlice::Point(0.0f, 0.0f),
                               ReferenceScale)
                        .HeightOffset;
                  }),
          }),
      Seed.HeightOffset);
}

inline FLevelLocalPoint LabelLocalPointFromSeed(
    const FLevelRuntimeLabelSeed &Seed, const FVector &ReferenceScale) {
  const float HeightOffset = LabelHeightOffsetFromSeed(Seed, ReferenceScale);
  return LevelLayoutSlice::FromPostOfficeLots(Seed.EastLots, Seed.NorthLots,
                                             HeightOffset);
}

inline FVector LabelLocationForLandmark(const FLandmark &Landmark) {
  return Landmark.Location + FVector::UpVector *
                                 (LevelLayoutSlice::CubeHalfExtent() *
                                      Landmark.Scale.Z +
                                  LevelLayoutSlice::BuildingFoundationHeight() +
                                  LevelLayoutSlice::CubeHalfExtent());
}

inline ELevelRetroTexture TextureForNatureKind(ENatureFeatureKind Kind) {
  return func::or_else(
      func::multi_match<ENatureFeatureKind, ELevelRetroTexture>(
          Kind,
          {
              func::when<ENatureFeatureKind, ELevelRetroTexture>(
                  func::equals<ENatureFeatureKind>(ENatureFeatureKind::Water),
                  [](ENatureFeatureKind) {
                    return ELevelRetroTexture::WaterCreek;
                  }),
              func::when<ENatureFeatureKind, ELevelRetroTexture>(
                  func::equals<ENatureFeatureKind>(ENatureFeatureKind::Rock),
                  [](ENatureFeatureKind) {
                    return ELevelRetroTexture::RockGranite;
                  }),
              func::when<ENatureFeatureKind, ELevelRetroTexture>(
                  func::equals<ENatureFeatureKind>(
                      ENatureFeatureKind::TreeGrove),
                  [](ENatureFeatureKind) {
                    return ELevelRetroTexture::FoliageRiparian;
                  }),
              func::when<ENatureFeatureKind, ELevelRetroTexture>(
                  func::equals<ENatureFeatureKind>(ENatureFeatureKind::Shrub),
                  [](ENatureFeatureKind) {
                    return ELevelRetroTexture::FoliageRiparian;
                  }),
          }),
      ELevelRetroTexture::MarkerPaint);
}

inline bool NatureFeatureNeedsLabel(ENatureFeatureKind Kind) {
  return Kind == ENatureFeatureKind::PCGMarker ||
         Kind == ENatureFeatureKind::WaterSystemMarker;
}

template <typename Source, typename Output, typename Mapper>
inline TArray<Output> MapArrayRecursive(const TArray<Source> &Values,
                                        Mapper MapValue, int32 Index,
                                        TArray<Output> Acc) {
  return Index >= Values.Num()
             ? Acc
             : ([&Values, MapValue, Index](TArray<Output> Next) {
                 Next.Add(MapValue(Values[Index]));
                 return MapArrayRecursive<Source, Output>(
                     Values, MapValue, Index + 1, MoveTemp(Next));
               })(MoveTemp(Acc));
}

template <typename Source, typename Output, typename Mapper>
inline TArray<Output> MapArray(const TArray<Source> &Values, Mapper MapValue) {
  TArray<Output> Acc;
  Acc.Reserve(Values.Num());
  return MapArrayRecursive<Source, Output>(Values, MapValue, 0, MoveTemp(Acc));
}

template <typename Source, typename Output, typename Mapper,
          typename Predicate>
inline TArray<Output> FilterMapArrayRecursive(const TArray<Source> &Values,
                                              Predicate Keep,
                                              Mapper MapValue, int32 Index,
                                              TArray<Output> Acc) {
  return Index >= Values.Num()
             ? Acc
             : ([&Values, Keep, MapValue, Index](TArray<Output> Next) {
                 Keep(Values[Index]) && (Next.Add(MapValue(Values[Index])),
                                         true);
                 return FilterMapArrayRecursive<Source, Output>(
                     Values, Keep, MapValue, Index + 1, MoveTemp(Next));
               })(MoveTemp(Acc));
}

template <typename Source, typename Output, typename Mapper,
          typename Predicate>
inline TArray<Output> FilterMapArray(const TArray<Source> &Values,
                                     Predicate Keep, Mapper MapValue) {
  TArray<Output> Acc;
  Acc.Reserve(Values.Num());
  return FilterMapArrayRecursive<Source, Output>(
      Values, Keep, MapValue, 0, MoveTemp(Acc));
}

inline TArray<FVector> BuildWorldRouteRecursive(
    const TArray<FLevelLocalPoint> &Route, const FLevelTerrainData &TerrainData,
    int32 Index, TArray<FVector> Acc) {
  return Index >= Route.Num()
             ? Acc
             : ([&Route, &TerrainData, Index](TArray<FVector> Next) {
                 Next.Add(LevelLayoutSlice::ToWorld(TerrainData, Route[Index]));
                 return BuildWorldRouteRecursive(Route, TerrainData, Index + 1,
                                                 MoveTemp(Next));
               })(MoveTemp(Acc));
}

} // namespace detail

inline FLevelBlockSpawn
BuildRuntimeBlockSpawn(const FLevelRuntimeBlockSeed &Seed,
                       const FLevelTerrainData &TerrainData) {
  const FVector Scale = detail::ScaleFromSeed(Seed.Scale);
  return {Seed.Name,
          detail::WorldLocationFromSeed(Seed, TerrainData, Scale),
          Scale,
          Seed.Texture};
}

inline FLevelLabelSpawn
BuildRuntimeLabelSpawn(const FLevelRuntimeLabelSeed &Seed,
                       const FLevelTerrainData &TerrainData) {
  const FVector ReferenceScale = detail::ScaleFromSeed(Seed.ReferenceScale);
  return {Seed.Text,
          LevelLayoutSlice::ToWorld(
              TerrainData,
              detail::LabelLocalPointFromSeed(Seed, ReferenceScale)),
          LevelLayoutSlice::CubeHalfExtent() * Seed.WorldSizeScale};
}

inline FLevelRuntimeSectionSpawn
BuildRuntimeSectionSpawn(const FLevelRuntimeSectionSeed &Seed,
                         const FLevelTerrainData &TerrainData) {
  return {detail::MapArray<FLevelRuntimeBlockSeed, FLevelBlockSpawn>(
              Seed.Blocks,
              [&TerrainData](const FLevelRuntimeBlockSeed &BlockSeed) {
                return BuildRuntimeBlockSpawn(BlockSeed, TerrainData);
              }),
          detail::MapArray<FLevelRuntimeLabelSeed, FLevelLabelSpawn>(
              Seed.Labels,
              [&TerrainData](const FLevelRuntimeLabelSeed &LabelSeed) {
                return BuildRuntimeLabelSpawn(LabelSeed, TerrainData);
              })};
}

inline FLevelRuntimeSectionSpawn
BuildOverlaySectionSpawn(const FLevelRuntimeLayoutSeed &Seed,
                         const FLevelTerrainData &TerrainData) {
  return {TArray<FLevelBlockSpawn>(),
          detail::MapArray<FLevelRuntimeLabelSeed, FLevelLabelSpawn>(
              Seed.OverlayLabels,
              [&TerrainData](const FLevelRuntimeLabelSeed &LabelSeed) {
                return BuildRuntimeLabelSpawn(LabelSeed, TerrainData);
              })};
}

inline FLevelRuntimeSectionSpawn
BuildLandmarkSectionSpawn(const TArray<FLandmark> &Landmarks) {
  return {detail::MapArray<FLandmark, FLevelBlockSpawn>(
              Landmarks,
              [](const FLandmark &Landmark) {
                return FLevelBlockSpawn{Landmark.Label, Landmark.Location,
                                        Landmark.Scale,
                                        ELevelRetroTexture::BuildingTimber};
              }),
          detail::MapArray<FLandmark, FLevelLabelSpawn>(
              Landmarks,
              [](const FLandmark &Landmark) {
                return FLevelLabelSpawn{
                    Landmark.Label, detail::LabelLocationForLandmark(Landmark),
                    LevelLayoutSlice::CubeHalfExtent() * 0.48f};
              })};
}

inline FLevelRuntimeSectionSpawn
BuildNatureSectionSpawn(const TArray<FNatureFeatureSeed> &Features,
                        const FLevelTerrainData &TerrainData) {
  return {detail::MapArray<FNatureFeatureSeed, FLevelBlockSpawn>(
              Features,
              [&TerrainData](const FNatureFeatureSeed &Feature) {
                return FLevelBlockSpawn{
                    Feature.Name,
                    LevelLayoutSlice::ToWorld(TerrainData, Feature.Location),
                    Feature.Scale, detail::TextureForNatureKind(Feature.Kind)};
              }),
          detail::FilterMapArray<FNatureFeatureSeed, FLevelLabelSpawn>(
              Features,
              [](const FNatureFeatureSeed &Feature) {
                return detail::NatureFeatureNeedsLabel(Feature.Kind);
              },
              [&TerrainData](const FNatureFeatureSeed &Feature) {
                return FLevelLabelSpawn{
                    Feature.Name,
                    LevelLayoutSlice::ToWorld(
                        TerrainData,
                        LevelLayoutSlice::AboveBlock(Feature.Location,
                                                     Feature.Scale)),
                    LevelLayoutSlice::CubeHalfExtent() * 0.36f};
              })};
}

inline TArray<FVector>
BuildWorldRoute(const TArray<FLevelLocalPoint> &Route,
                const FLevelTerrainData &TerrainData) {
  TArray<FVector> Acc;
  Acc.Reserve(Route.Num());
  return detail::BuildWorldRouteRecursive(Route, TerrainData, 0,
                                          MoveTemp(Acc));
}

inline FLevelSystemState
ReduceLevelObserved(const FLevelSystemState &State,
                    const rtk::PayloadAction<FLevelSystemPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FLevelSystemState Next) -> FLevelSystemState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace LevelSystemReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
