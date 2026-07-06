#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Level/LevelTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemReducers {
namespace detail {

struct FGroundLotsRequest {
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
  FVector Scale = FVector::OneVector;
  float Clearance = 0.0f;
};

struct FScaleFromSeedRequest {
  FLevelRuntimeScaleSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLocalPointFromSeedRequest {
  FLevelRuntimeBlockSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FWorldLocationFromSeedRequest {
  FLevelRuntimeBlockSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FLabelHeightOffsetRequest {
  FLevelRuntimeLabelSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FVector ReferenceScale;
};

struct FLabelLocalPointRequest {
  FLevelRuntimeLabelSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLandmarkLabelLocationRequest {
  FLandmark Landmark;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

inline FLevelLocalPoint GroundLots(const FGroundLotsRequest &Request) {
  return LevelLayoutSlice::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutSlice::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
       Request.Scale, Request.Clearance});
}

inline FLevelLocalPoint BuildingLots(const FGroundLotsRequest &Request) {
  return GroundLots({Request.Geometry, Request.EastLots, Request.NorthLots,
                     Request.Scale,
                     LevelLayoutSlice::BuildingFoundationHeight(
                         Request.Geometry)});
}

inline FLevelLocalPoint FeatureLots(const FGroundLotsRequest &Request) {
  return GroundLots({Request.Geometry, Request.EastLots, Request.NorthLots,
                     Request.Scale,
                     LevelLayoutSlice::RoadSurfaceClearance(Request.Geometry)});
}

inline FVector ScaleFromSeed(const FScaleFromSeedRequest &Request) {
  return Request.Seed.Mode == ELevelRuntimeScaleMode::Building
             ? LevelLayoutSlice::BuildingScaleFromFeet(
                   {Request.Geometry, Request.Seed.FrontageFeet,
                    Request.Seed.DepthFeet, Request.Seed.Stories})
             : Request.Seed.Mode == ELevelRuntimeScaleMode::LongFeature
                   ? LevelLayoutSlice::LongFeatureScale(
                         {Request.Geometry, Request.Seed.WidthFeet,
                          Request.Seed.LengthLots, Request.Seed.HeightFeet})
                   : LevelLayoutSlice::PadScaleFromFeet(
                         {Request.Geometry, Request.Seed.WidthFeet,
                          Request.Seed.DepthFeet, Request.Seed.HeightFeet});
}

inline FLevelLocalPoint LocalPointFromSeed(
    const FLocalPointFromSeedRequest &Request) {
  return Request.Seed.Anchor == ELevelRuntimeAnchorMode::BuildingLots
             ? BuildingLots({Request.Geometry, Request.Seed.EastLots,
                             Request.Seed.NorthLots, Request.Scale, 0.0f})
             : Request.Seed.Anchor == ELevelRuntimeAnchorMode::PostOfficeLots
                   ? LevelLayoutSlice::FromPostOfficeLots(
                         {Request.Geometry, Request.Seed.EastLots,
                          Request.Seed.NorthLots, 0.0f})
                   : FeatureLots({Request.Geometry, Request.Seed.EastLots,
                                  Request.Seed.NorthLots, Request.Scale,
                                  0.0f});
}

inline FVector WorldLocationFromSeed(
    const FWorldLocationFromSeedRequest &Request) {
  return Request.Seed.Anchor == ELevelRuntimeAnchorMode::World
             ? Request.Seed.WorldLocation
             : LevelLayoutSlice::ToWorld(
                   {Request.TerrainData,
                    LocalPointFromSeed({Request.Seed, Request.Geometry,
                                        Request.Scale})});
}

inline float LabelHeightOffsetFromSeed(
    const FLabelHeightOffsetRequest &Request) {
  return Request.Seed.Height == ELevelRuntimeLabelHeightMode::LabelForScale
             ? LevelLayoutSlice::LabelHeightForScale(
                   {Request.Geometry, Request.ReferenceScale})
             : Request.Seed.Height == ELevelRuntimeLabelHeightMode::AboveBlock
                   ? LevelLayoutSlice::AboveBlock(
                         {Request.Geometry,
                          LevelLayoutSlice::Point({0.0f, 0.0f, 0.0f}),
                          Request.ReferenceScale})
                         .HeightOffset
                   : Request.Seed.HeightOffset;
}

inline FLevelLocalPoint ExplicitLabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  return LevelLayoutSlice::FromPostOfficeLots(
      {Request.Geometry, Request.Seed.EastLots, Request.Seed.NorthLots,
       Request.Seed.HeightOffset});
}

inline FLevelLocalPoint ReferenceLabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  const FVector ReferenceScale =
      ScaleFromSeed({Request.Seed.ReferenceScale, Request.Geometry});
  const float HeightOffset = LabelHeightOffsetFromSeed(
      {Request.Seed, Request.Geometry, ReferenceScale});
  return LevelLayoutSlice::FromPostOfficeLots(
      {Request.Geometry, Request.Seed.EastLots, Request.Seed.NorthLots,
       HeightOffset});
}

inline FLevelLocalPoint LabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  return Request.Seed.Height == ELevelRuntimeLabelHeightMode::Explicit
             ? ExplicitLabelLocalPointFromSeed(Request)
             : ReferenceLabelLocalPointFromSeed(Request);
}

inline FVector LabelLocationForLandmark(
    const FLandmarkLabelLocationRequest &Request) {
  return Request.Landmark.Location +
         FVector::UpVector *
             (LevelLayoutSlice::CubeHalfExtent(Request.Geometry) *
                  Request.Landmark.Scale.Z +
              LevelLayoutSlice::BuildingFoundationHeight(Request.Geometry) +
              LevelLayoutSlice::CubeHalfExtent(Request.Geometry));
}

inline ELevelRetroTexture TextureForNatureKind(ENatureFeatureKind Kind) {
  const func::Maybe<ELevelRetroTexture> Texture =
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
              func::when<ENatureFeatureKind, ELevelRetroTexture>(
                  func::equals<ENatureFeatureKind>(
                      ENatureFeatureKind::PCGMarker),
                  [](ENatureFeatureKind) {
                    return ELevelRetroTexture::MarkerPaint;
                  }),
              func::when<ENatureFeatureKind, ELevelRetroTexture>(
                  func::equals<ENatureFeatureKind>(
                      ENatureFeatureKind::WaterSystemMarker),
                  [](ENatureFeatureKind) {
                    return ELevelRetroTexture::MarkerPaint;
                  }),
          });
  checkf(Texture.hasValue, TEXT("Unhandled nature feature kind"));
  return Texture.value;
}

inline bool NatureFeatureNeedsLabel(ENatureFeatureKind Kind) {
  return Kind == ENatureFeatureKind::PCGMarker ||
         Kind == ENatureFeatureKind::WaterSystemMarker;
}

} // namespace detail

inline FLevelBlockSpawn
BuildRuntimeBlockSpawn(const FLevelRuntimeBlockSpawnRequest &Request) {
  const FVector Scale =
      detail::ScaleFromSeed({Request.Seed.Scale, Request.Geometry});
  return {Request.Seed.Name,
          detail::WorldLocationFromSeed(
              {Request.Seed, Request.TerrainData, Request.Geometry, Scale}),
          Scale,
          Request.Seed.Texture};
}

inline FLevelLabelSpawn
BuildRuntimeLabelSpawn(const FLevelRuntimeLabelSpawnRequest &Request) {
  return {Request.Seed.Text,
          LevelLayoutSlice::ToWorld(
              {Request.TerrainData,
               detail::LabelLocalPointFromSeed(
                   {Request.Seed, Request.Geometry})}),
          LevelLayoutSlice::CubeHalfExtent(Request.Geometry) *
              Request.Seed.WorldSizeScale};
}

inline FLevelRuntimeSectionSpawn
BuildRuntimeSectionSpawn(const FLevelRuntimeSectionSpawnRequest &Request) {
  return {func::map_array<FLevelRuntimeBlockSeed, FLevelBlockSpawn>(
              Request.Seed.Blocks,
              [&Request](const FLevelRuntimeBlockSeed &BlockSeed) {
                return BuildRuntimeBlockSpawn(
                    {BlockSeed, Request.TerrainData, Request.Geometry});
              }),
          func::map_array<FLevelRuntimeLabelSeed, FLevelLabelSpawn>(
              Request.Seed.Labels,
              [&Request](const FLevelRuntimeLabelSeed &LabelSeed) {
                return BuildRuntimeLabelSpawn(
                    {LabelSeed, Request.TerrainData, Request.Geometry});
              })};
}

inline FLevelRuntimeSectionSpawn
BuildOverlaySectionSpawn(const FLevelOverlaySectionSpawnRequest &Request) {
  return {TArray<FLevelBlockSpawn>(),
          func::map_array<FLevelRuntimeLabelSeed, FLevelLabelSpawn>(
              Request.Seed.OverlayLabels,
              [&Request](const FLevelRuntimeLabelSeed &LabelSeed) {
                return BuildRuntimeLabelSpawn(
                    {LabelSeed, Request.TerrainData, Request.Geometry});
              })};
}

inline FLevelRuntimeSectionSpawn
BuildLandmarkSectionSpawn(const FLevelLandmarkSectionSpawnRequest &Request) {
  return {func::map_array<FLandmark, FLevelBlockSpawn>(
              Request.Landmarks,
              [](const FLandmark &Landmark) {
                return FLevelBlockSpawn{Landmark.Label, Landmark.Location,
                                        Landmark.Scale,
                                        ELevelRetroTexture::BuildingTimber};
              }),
          func::map_array<FLandmark, FLevelLabelSpawn>(
              Request.Landmarks,
              [&Request](const FLandmark &Landmark) {
                return FLevelLabelSpawn{
                    Landmark.Label,
                    detail::LabelLocationForLandmark(
                        {Landmark, Request.Geometry}),
                    LevelLayoutSlice::CubeHalfExtent(Request.Geometry) *
                        Request.Geometry.LandmarkLabelWorldSizeScale};
              })};
}

inline FLevelRuntimeSectionSpawn
BuildNatureSectionSpawn(const FLevelNatureSectionSpawnRequest &Request) {
  return {func::map_array<FNatureFeatureSeed, FLevelBlockSpawn>(
              Request.Features,
              [&Request](const FNatureFeatureSeed &Feature) {
                return FLevelBlockSpawn{
                    Feature.Name,
                    LevelLayoutSlice::ToWorld(
                        {Request.TerrainData, Feature.Location}),
                    Feature.Scale, detail::TextureForNatureKind(Feature.Kind)};
              }),
          func::filter_map_array<FNatureFeatureSeed, FLevelLabelSpawn>(
              Request.Features,
              [](const FNatureFeatureSeed &Feature) {
                return detail::NatureFeatureNeedsLabel(Feature.Kind);
              },
              [&Request](const FNatureFeatureSeed &Feature) {
                return FLevelLabelSpawn{
                    Feature.Name,
                    LevelLayoutSlice::ToWorld(
                        {Request.TerrainData,
                         LevelLayoutSlice::AboveBlock(
                             {Request.Geometry, Feature.Location,
                              Feature.Scale})}),
                    LevelLayoutSlice::CubeHalfExtent(Request.Geometry) *
                        Request.Geometry.NatureLabelWorldSizeScale};
              })};
}

inline TArray<FVector>
BuildWorldRoute(const FLevelWorldRouteRequest &Request) {
  return func::map_array<FLevelLocalPoint, FVector>(
      Request.Route, [&Request](const FLevelLocalPoint &Point) {
        return LevelLayoutSlice::ToWorld({Request.TerrainData, Point});
      });
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
} // namespace Game
} // namespace ForbocAI
