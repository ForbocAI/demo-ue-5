#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"
#include "Features/Systems/Level/SystemsLevelTypes.h"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemReducers {
namespace detail {

struct FGroundLotsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
  FVector Scale = FVector::OneVector;
  float Clearance = 0.0f;
};

struct FScaleFromSeedRequest {
  FScaleSeed Seed;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FLocalPointFromSeedRequest {
  FBlockSeed Seed;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FWorldLocationFromSeedRequest {
  FBlockSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FLabelHeightOffsetRequest {
  FLabelSeed Seed;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FVector ReferenceScale;
};

struct FLabelLocalPointRequest {
  FLabelSeed Seed;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FLandmarkLabelLocationRequest {
  FLandmark Landmark;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

inline FLevelLocalPoint GroundLots(const FGroundLotsRequest &Request) {
  return LevelLayoutAdapters::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutAdapters::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
       Request.Scale, Request.Clearance});
}

inline FLevelLocalPoint BuildingLots(const FGroundLotsRequest &Request) {
  return GroundLots({Request.Geometry, Request.EastLots, Request.NorthLots,
                     Request.Scale,
                     LevelLayoutAdapters::BuildingFoundationHeight(
                         Request.Geometry)});
}

inline FLevelLocalPoint FeatureLots(const FGroundLotsRequest &Request) {
  return GroundLots({Request.Geometry, Request.EastLots, Request.NorthLots,
                     Request.Scale,
                     LevelLayoutAdapters::RoadSurfaceClearance(Request.Geometry)});
}

inline FVector ScaleFromSeed(const FScaleFromSeedRequest &Request) {
  return Request.Seed.Mode == EScaleMode::Building
             ? LevelLayoutAdapters::BuildingScaleFromFeet(
                   {Request.Geometry, Request.Seed.FrontageFeet,
                    Request.Seed.DepthFeet, Request.Seed.Stories})
             : Request.Seed.Mode == EScaleMode::LongFeature
                   ? LevelLayoutAdapters::LongFeatureScale(
                         {Request.Geometry, Request.Seed.WidthFeet,
                          Request.Seed.LengthLots, Request.Seed.HeightFeet})
                   : LevelLayoutAdapters::PadScaleFromFeet(
                         {Request.Geometry, Request.Seed.WidthFeet,
                          Request.Seed.DepthFeet, Request.Seed.HeightFeet});
}

inline FLevelLocalPoint LocalPointFromSeed(
    const FLocalPointFromSeedRequest &Request) {
  return Request.Seed.Anchor == EAnchorMode::BuildingLots
             ? BuildingLots({Request.Geometry, Request.Seed.EastLots,
                             Request.Seed.NorthLots, Request.Scale, 0.0f})
             : Request.Seed.Anchor == EAnchorMode::PostOfficeLots
                   ? LevelLayoutAdapters::FromPostOfficeLots(
                         {Request.Geometry, Request.Seed.EastLots,
                          Request.Seed.NorthLots, 0.0f})
                   : FeatureLots({Request.Geometry, Request.Seed.EastLots,
                                  Request.Seed.NorthLots, Request.Scale,
                                  0.0f});
}

inline FVector WorldLocationFromSeed(
    const FWorldLocationFromSeedRequest &Request) {
  return Request.Seed.Anchor == EAnchorMode::World
             ? Request.Seed.WorldLocation
             : LevelLayoutAdapters::ToWorld(
                   {Request.TerrainData,
                    LocalPointFromSeed({Request.Seed, Request.Geometry,
                                        Request.Scale})});
}

inline float LabelHeightOffsetFromSeed(
    const FLabelHeightOffsetRequest &Request) {
  return Request.Seed.Height == ELabelHeightMode::LabelForScale
             ? LevelLayoutAdapters::LabelHeightForScale(
                   {Request.Geometry, Request.ReferenceScale})
             : Request.Seed.Height == ELabelHeightMode::AboveBlock
                   ? LevelLayoutAdapters::AboveBlock(
                         {Request.Geometry,
                          LevelLayoutAdapters::Point({0.0f, 0.0f, 0.0f}),
                          Request.ReferenceScale})
                         .HeightOffset
                   : Request.Seed.HeightOffset;
}

inline FLevelLocalPoint ExplicitLabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  return LevelLayoutAdapters::FromPostOfficeLots(
      {Request.Geometry, Request.Seed.EastLots, Request.Seed.NorthLots,
       Request.Seed.HeightOffset});
}

inline FLevelLocalPoint ReferenceLabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  const FVector ReferenceScale =
      ScaleFromSeed({Request.Seed.ReferenceScale, Request.Geometry});
  const float HeightOffset = LabelHeightOffsetFromSeed(
      {Request.Seed, Request.Geometry, ReferenceScale});
  return LevelLayoutAdapters::FromPostOfficeLots(
      {Request.Geometry, Request.Seed.EastLots, Request.Seed.NorthLots,
       HeightOffset});
}

inline FLevelLocalPoint LabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  return Request.Seed.Height == ELabelHeightMode::Explicit
             ? ExplicitLabelLocalPointFromSeed(Request)
             : ReferenceLabelLocalPointFromSeed(Request);
}

inline FVector LabelLocationForLandmark(
    const FLandmarkLabelLocationRequest &Request) {
  return Request.Landmark.Location +
         FVector::UpVector *
             (LevelLayoutAdapters::CubeHalfExtent(Request.Geometry) *
                  Request.Landmark.Scale.Z +
              LevelLayoutAdapters::BuildingFoundationHeight(Request.Geometry) +
              LevelLayoutAdapters::CubeHalfExtent(Request.Geometry));
}

inline ELevelRetroTexture TextureForNatureKind(EFeatureKind Kind) {
  const func::Maybe<ELevelRetroTexture> Texture =
      func::multi_match<EFeatureKind, ELevelRetroTexture>(
          Kind,
          {
              func::when<EFeatureKind, ELevelRetroTexture>(
                  func::equals<EFeatureKind>(EFeatureKind::Water),
                  [](EFeatureKind) {
                    return ELevelRetroTexture::WaterCreek;
                  }),
              func::when<EFeatureKind, ELevelRetroTexture>(
                  func::equals<EFeatureKind>(EFeatureKind::Rock),
                  [](EFeatureKind) {
                    return ELevelRetroTexture::RockGranite;
                  }),
              func::when<EFeatureKind, ELevelRetroTexture>(
                  func::equals<EFeatureKind>(
                      EFeatureKind::TreeGrove),
                  [](EFeatureKind) {
                    return ELevelRetroTexture::FoliageRiparian;
                  }),
              func::when<EFeatureKind, ELevelRetroTexture>(
                  func::equals<EFeatureKind>(EFeatureKind::Shrub),
                  [](EFeatureKind) {
                    return ELevelRetroTexture::FoliageRiparian;
                  }),
              func::when<EFeatureKind, ELevelRetroTexture>(
                  func::equals<EFeatureKind>(
                      EFeatureKind::PCGMarker),
                  [](EFeatureKind) {
                    return ELevelRetroTexture::MarkerPaint;
                  }),
              func::when<EFeatureKind, ELevelRetroTexture>(
                  func::equals<EFeatureKind>(
                      EFeatureKind::WaterSystemMarker),
                  [](EFeatureKind) {
                    return ELevelRetroTexture::MarkerPaint;
                  }),
          });
  checkf(Texture.hasValue, TEXT("Unhandled nature feature kind"));
  return Texture.value;
}

inline bool NatureFeatureNeedsLabel(EFeatureKind Kind) {
  return Kind == EFeatureKind::PCGMarker ||
         Kind == EFeatureKind::WaterSystemMarker;
}

} // namespace detail

inline FBlockSpawn
BuildBlockSpawn(const FBlockSpawnRequest &Request) {
  const FVector Scale =
      detail::ScaleFromSeed({Request.Seed.Scale, Request.Geometry});
  return {Request.Seed.Name,
          detail::WorldLocationFromSeed(
              {Request.Seed, Request.TerrainData, Request.Geometry, Scale}),
          FRotator(0.0f, Request.Seed.YawDegrees, 0.0f),
          Scale,
          Request.Seed.Texture};
}

inline FLabelSpawn
BuildLabelSpawn(const FLabelSpawnRequest &Request) {
  return {Request.Seed.Text,
          LevelLayoutAdapters::ToWorld(
              {Request.TerrainData,
               detail::LabelLocalPointFromSeed(
                   {Request.Seed, Request.Geometry})}),
          LevelLayoutAdapters::CubeHalfExtent(Request.Geometry) *
              Request.Seed.WorldSizeScale};
}

inline FSectionSpawn
BuildSectionSpawn(const FSectionSpawnRequest &Request) {
  return {func::map_array<FBlockSeed, FBlockSpawn>(
              Request.Seed.Blocks,
              [&Request](const FBlockSeed &BlockSeed) {
                return BuildBlockSpawn(
                    {BlockSeed, Request.TerrainData, Request.Geometry});
              }),
          func::map_array<FLabelSeed, FLabelSpawn>(
              Request.Seed.Labels,
              [&Request](const FLabelSeed &LabelSeed) {
                return BuildLabelSpawn(
                    {LabelSeed, Request.TerrainData, Request.Geometry});
              })};
}

inline FSectionSpawn
BuildOverlaySectionSpawn(const FOverlaySectionSpawnRequest &Request) {
  return {TArray<FBlockSpawn>(),
          func::map_array<FLabelSeed, FLabelSpawn>(
              Request.Seed.OverlayLabels,
              [&Request](const FLabelSeed &LabelSeed) {
                return BuildLabelSpawn(
                    {LabelSeed, Request.TerrainData, Request.Geometry});
              })};
}

inline FSectionSpawn
BuildLandmarkSectionSpawn(const FLandmarkSectionSpawnRequest &Request) {
  return {func::map_array<FLandmark, FBlockSpawn>(
              Request.Landmarks,
              [](const FLandmark &Landmark) {
                return FBlockSpawn{Landmark.Label, Landmark.Location,
                                        Landmark.Rotation, Landmark.Scale,
                                        ELevelRetroTexture::BuildingTimber};
              }),
          func::map_array<FLandmark, FLabelSpawn>(
              Request.Landmarks,
              [&Request](const FLandmark &Landmark) {
                return FLabelSpawn{
                    Landmark.Label,
                    detail::LabelLocationForLandmark(
                        {Landmark, Request.Geometry}),
                    LevelLayoutAdapters::CubeHalfExtent(Request.Geometry) *
                        Request.Geometry.LandmarkLabelWorldSizeScale};
              })};
}

inline FSectionSpawn
BuildNatureSectionSpawn(const FNatureSectionSpawnRequest &Request) {
  return {func::map_array<FFeatureSeed, FBlockSpawn>(
              Request.Features,
              [&Request](const FFeatureSeed &Feature) {
                return FBlockSpawn{
                    Feature.Name,
                    LevelLayoutAdapters::ToWorld(
                        {Request.TerrainData, Feature.Location}),
                    FRotator::ZeroRotator,
                    Feature.Scale, detail::TextureForNatureKind(Feature.Kind)};
              }),
          func::filter_map_array<FFeatureSeed, FLabelSpawn>(
              Request.Features,
              [](const FFeatureSeed &Feature) {
                return detail::NatureFeatureNeedsLabel(Feature.Kind);
              },
              [&Request](const FFeatureSeed &Feature) {
                return FLabelSpawn{
                    Feature.Name,
                    LevelLayoutAdapters::ToWorld(
                        {Request.TerrainData,
                         LevelLayoutAdapters::AboveBlock(
                             {Request.Geometry, Feature.Location,
                              Feature.Scale})}),
                    LevelLayoutAdapters::CubeHalfExtent(Request.Geometry) *
                        Request.Geometry.NatureLabelWorldSizeScale};
              })};
}

inline TArray<FVector>
BuildWorldRoute(const FWorldRouteRequest &Request) {
  return func::map_array<FLevelLocalPoint, FVector>(
      Request.Route, [&Request](const FLevelLocalPoint &Point) {
        return LevelLayoutAdapters::ToWorld({Request.TerrainData, Point});
      });
}

inline FSystemState
ReduceLevelObserved(const FSystemState &State,
                    const rtk::PayloadAction<FSystemPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FSystemState Next) -> FSystemState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace LevelSystemReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelActions.h"
#include "Features/Systems/Level/SystemsLevelSelectors.h"
#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemSlice {

inline FSystemState CreateInitialState() {
  return (func::pipe(FSystemState{}) |
          [](FSystemState State) -> FSystemState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FSystemState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FSystemState>> Slice =
      func::lazy([]() -> rtk::Slice<FSystemState> {
        // RTK guidance: slice names are reducer/action metadata, not authored gameplay data.
        return rtk::createSlice<FSystemState>(
            TEXT("systems/level"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FSystemState>
                   &Builder) {
              Builder.addCase(LevelSystemActions::LevelObserved(),
                              LevelSystemReducers::ReduceLevelObserved);
            });
      });
  return func::eval(Slice);
}

inline FSectionSpawn
BuildSectionSpawn(const FSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildSectionSpawn(Request);
}

inline FSectionSpawn
BuildOverlaySectionSpawn(const FOverlaySectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildOverlaySectionSpawn(Request);
}

inline FSectionSpawn
BuildLandmarkSectionSpawn(const FLandmarkSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildLandmarkSectionSpawn(Request);
}

inline FSectionSpawn
BuildNatureSectionSpawn(const FNatureSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildNatureSectionSpawn(Request);
}

inline TArray<FVector> BuildWorldRoute(const FWorldRouteRequest &Request) {
  return LevelSystemReducers::BuildWorldRoute(Request);
}

} // namespace LevelSystemSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
