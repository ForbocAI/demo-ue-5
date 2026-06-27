#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataAdapters.h"
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
BuildRuntimeBlockSpawn(const FLevelRuntimeBlockSpawnRequest &Request) {
  const FVector Scale = detail::ScaleFromSeed(Request.Seed.Scale);
  return {Request.Seed.Name,
          detail::WorldLocationFromSeed(Request.Seed, Request.TerrainData,
                                        Scale),
          Scale,
          Request.Seed.Texture};
}

inline FLevelLabelSpawn
BuildRuntimeLabelSpawn(const FLevelRuntimeLabelSpawnRequest &Request) {
  const FVector ReferenceScale =
      detail::ScaleFromSeed(Request.Seed.ReferenceScale);
  return {Request.Seed.Text,
          LevelLayoutSlice::ToWorld(
              Request.TerrainData,
              detail::LabelLocalPointFromSeed(Request.Seed, ReferenceScale)),
          LevelLayoutSlice::CubeHalfExtent() * Request.Seed.WorldSizeScale};
}

inline FLevelRuntimeSectionSpawn
BuildRuntimeSectionSpawn(const FLevelRuntimeSectionSpawnRequest &Request) {
  return {Data::DataAdapters::MapArray<FLevelRuntimeBlockSeed,
                                       FLevelBlockSpawn>(
              {Request.Seed.Blocks,
               [&Request](const FLevelRuntimeBlockSeed &BlockSeed) {
                return BuildRuntimeBlockSpawn(
                    {BlockSeed, Request.TerrainData});
              }}),
          Data::DataAdapters::MapArray<FLevelRuntimeLabelSeed,
                                       FLevelLabelSpawn>(
              {Request.Seed.Labels,
               [&Request](const FLevelRuntimeLabelSeed &LabelSeed) {
                return BuildRuntimeLabelSpawn(
                    {LabelSeed, Request.TerrainData});
              }})};
}

inline FLevelRuntimeSectionSpawn
BuildOverlaySectionSpawn(const FLevelOverlaySectionSpawnRequest &Request) {
  return {TArray<FLevelBlockSpawn>(),
          Data::DataAdapters::MapArray<FLevelRuntimeLabelSeed,
                                       FLevelLabelSpawn>(
              {Request.Seed.OverlayLabels,
               [&Request](const FLevelRuntimeLabelSeed &LabelSeed) {
                return BuildRuntimeLabelSpawn(
                    {LabelSeed, Request.TerrainData});
              }})};
}

inline FLevelRuntimeSectionSpawn
BuildLandmarkSectionSpawn(const TArray<FLandmark> &Landmarks) {
  return {Data::DataAdapters::MapArray<FLandmark, FLevelBlockSpawn>(
              {Landmarks,
               [](const FLandmark &Landmark) {
                return FLevelBlockSpawn{Landmark.Label, Landmark.Location,
                                        Landmark.Scale,
                                        ELevelRetroTexture::BuildingTimber};
              }}),
          Data::DataAdapters::MapArray<FLandmark, FLevelLabelSpawn>(
              {Landmarks,
               [](const FLandmark &Landmark) {
                return FLevelLabelSpawn{
                    Landmark.Label, detail::LabelLocationForLandmark(Landmark),
                    LevelLayoutSlice::CubeHalfExtent() * 0.48f};
              }})};
}

inline FLevelRuntimeSectionSpawn
BuildNatureSectionSpawn(const FLevelNatureSectionSpawnRequest &Request) {
  return {Data::DataAdapters::MapArray<FNatureFeatureSeed, FLevelBlockSpawn>(
              {Request.Features,
               [&Request](const FNatureFeatureSeed &Feature) {
                return FLevelBlockSpawn{
                    Feature.Name,
                    LevelLayoutSlice::ToWorld(Request.TerrainData,
                                             Feature.Location),
                    Feature.Scale, detail::TextureForNatureKind(Feature.Kind)};
              }}),
          Data::DataAdapters::FilterMapArray<FNatureFeatureSeed,
                                             FLevelLabelSpawn>(
              {Request.Features,
               [](const FNatureFeatureSeed &Feature) {
                return detail::NatureFeatureNeedsLabel(Feature.Kind);
              },
               [&Request](const FNatureFeatureSeed &Feature) {
                return FLevelLabelSpawn{
                    Feature.Name,
                    LevelLayoutSlice::ToWorld(
                        Request.TerrainData,
                        LevelLayoutSlice::AboveBlock(Feature.Location,
                                                     Feature.Scale)),
                    LevelLayoutSlice::CubeHalfExtent() * 0.36f};
              }})};
}

inline TArray<FVector>
BuildWorldRoute(const FLevelWorldRouteRequest &Request) {
  TArray<FVector> Acc;
  Acc.Reserve(Request.Route.Num());
  return detail::BuildWorldRouteRecursive(
      Request.Route, Request.TerrainData, 0, MoveTemp(Acc));
}

inline FLevelBlockSpawn
BuildRuntimeBlockSpawn(const FLevelRuntimeBlockSeed &Seed,
                       const FLevelTerrainData &TerrainData) {
  return BuildRuntimeBlockSpawn({Seed, TerrainData});
}

inline FLevelLabelSpawn
BuildRuntimeLabelSpawn(const FLevelRuntimeLabelSeed &Seed,
                       const FLevelTerrainData &TerrainData) {
  return BuildRuntimeLabelSpawn({Seed, TerrainData});
}

inline FLevelRuntimeSectionSpawn
BuildRuntimeSectionSpawn(const FLevelRuntimeSectionSeed &Seed,
                         const FLevelTerrainData &TerrainData) {
  return BuildRuntimeSectionSpawn({Seed, TerrainData});
}

inline FLevelRuntimeSectionSpawn
BuildOverlaySectionSpawn(const FLevelRuntimeLayoutSeed &Seed,
                         const FLevelTerrainData &TerrainData) {
  return BuildOverlaySectionSpawn({Seed, TerrainData});
}

inline FLevelRuntimeSectionSpawn
BuildNatureSectionSpawn(const TArray<FNatureFeatureSeed> &Features,
                        const FLevelTerrainData &TerrainData) {
  return BuildNatureSectionSpawn({Features, TerrainData});
}

inline TArray<FVector>
BuildWorldRoute(const TArray<FLevelLocalPoint> &Route,
                const FLevelTerrainData &TerrainData) {
  return BuildWorldRoute({Route, TerrainData});
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
