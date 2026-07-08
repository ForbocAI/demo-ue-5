#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Components/Spatial/Level/Layout/Adapters.h"
#include "Features/Systems/Landmarks/Types.h"
#include "Features/Systems/Level/Types.h"
#include "Features/Systems/Nature/Types.h"

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
  FLevelScaleSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLocalPointFromSeedRequest {
  FLevelBlockSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FWorldLocationFromSeedRequest {
  FLevelBlockSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FLabelHeightOffsetRequest {
  FLevelLabelSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FVector ReferenceScale;
};

struct FLabelLocalPointRequest {
  FLevelLabelSeed Seed;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLandmarkLabelLocationRequest {
  FLandmark Landmark;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
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
  return Request.Seed.Mode == ELevelScaleMode::Building
             ? LevelLayoutAdapters::BuildingScaleFromFeet(
                   {Request.Geometry, Request.Seed.FrontageFeet,
                    Request.Seed.DepthFeet, Request.Seed.Stories})
             : Request.Seed.Mode == ELevelScaleMode::LongFeature
                   ? LevelLayoutAdapters::LongFeatureScale(
                         {Request.Geometry, Request.Seed.WidthFeet,
                          Request.Seed.LengthLots, Request.Seed.HeightFeet})
                   : LevelLayoutAdapters::PadScaleFromFeet(
                         {Request.Geometry, Request.Seed.WidthFeet,
                          Request.Seed.DepthFeet, Request.Seed.HeightFeet});
}

inline FLevelLocalPoint LocalPointFromSeed(
    const FLocalPointFromSeedRequest &Request) {
  return Request.Seed.Anchor == ELevelAnchorMode::BuildingLots
             ? BuildingLots({Request.Geometry, Request.Seed.EastLots,
                             Request.Seed.NorthLots, Request.Scale, 0.0f})
             : Request.Seed.Anchor == ELevelAnchorMode::PostOfficeLots
                   ? LevelLayoutAdapters::FromPostOfficeLots(
                         {Request.Geometry, Request.Seed.EastLots,
                          Request.Seed.NorthLots, 0.0f})
                   : FeatureLots({Request.Geometry, Request.Seed.EastLots,
                                  Request.Seed.NorthLots, Request.Scale,
                                  0.0f});
}

inline FVector WorldLocationFromSeed(
    const FWorldLocationFromSeedRequest &Request) {
  return Request.Seed.Anchor == ELevelAnchorMode::World
             ? Request.Seed.WorldLocation
             : LevelLayoutAdapters::ToWorld(
                   {Request.TerrainData,
                    LocalPointFromSeed({Request.Seed, Request.Geometry,
                                        Request.Scale})});
}

inline float LabelHeightOffsetFromSeed(
    const FLabelHeightOffsetRequest &Request) {
  return Request.Seed.Height == ELevelLabelHeightMode::LabelForScale
             ? LevelLayoutAdapters::LabelHeightForScale(
                   {Request.Geometry, Request.ReferenceScale})
             : Request.Seed.Height == ELevelLabelHeightMode::AboveBlock
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
  return Request.Seed.Height == ELevelLabelHeightMode::Explicit
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
BuildBlockSpawn(const FLevelBlockSpawnRequest &Request) {
  const FVector Scale =
      detail::ScaleFromSeed({Request.Seed.Scale, Request.Geometry});
  return {Request.Seed.Name,
          detail::WorldLocationFromSeed(
              {Request.Seed, Request.TerrainData, Request.Geometry, Scale}),
          FRotator(0.0f, Request.Seed.YawDegrees, 0.0f),
          Scale,
          Request.Seed.Texture};
}

inline FLevelLabelSpawn
BuildLabelSpawn(const FLevelLabelSpawnRequest &Request) {
  return {Request.Seed.Text,
          LevelLayoutAdapters::ToWorld(
              {Request.TerrainData,
               detail::LabelLocalPointFromSeed(
                   {Request.Seed, Request.Geometry})}),
          LevelLayoutAdapters::CubeHalfExtent(Request.Geometry) *
              Request.Seed.WorldSizeScale};
}

inline FLevelSectionSpawn
BuildSectionSpawn(const FLevelSectionSpawnRequest &Request) {
  return {func::map_array<FLevelBlockSeed, FLevelBlockSpawn>(
              Request.Seed.Blocks,
              [&Request](const FLevelBlockSeed &BlockSeed) {
                return BuildBlockSpawn(
                    {BlockSeed, Request.TerrainData, Request.Geometry});
              }),
          func::map_array<FLevelLabelSeed, FLevelLabelSpawn>(
              Request.Seed.Labels,
              [&Request](const FLevelLabelSeed &LabelSeed) {
                return BuildLabelSpawn(
                    {LabelSeed, Request.TerrainData, Request.Geometry});
              })};
}

inline FLevelSectionSpawn
BuildOverlaySectionSpawn(const FLevelOverlaySectionSpawnRequest &Request) {
  return {TArray<FLevelBlockSpawn>(),
          func::map_array<FLevelLabelSeed, FLevelLabelSpawn>(
              Request.Seed.OverlayLabels,
              [&Request](const FLevelLabelSeed &LabelSeed) {
                return BuildLabelSpawn(
                    {LabelSeed, Request.TerrainData, Request.Geometry});
              })};
}

inline FLevelSectionSpawn
BuildLandmarkSectionSpawn(const FLevelLandmarkSectionSpawnRequest &Request) {
  return {func::map_array<FLandmark, FLevelBlockSpawn>(
              Request.Landmarks,
              [](const FLandmark &Landmark) {
                return FLevelBlockSpawn{Landmark.Label, Landmark.Location,
                                        Landmark.Rotation, Landmark.Scale,
                                        ELevelRetroTexture::BuildingTimber};
              }),
          func::map_array<FLandmark, FLevelLabelSpawn>(
              Request.Landmarks,
              [&Request](const FLandmark &Landmark) {
                return FLevelLabelSpawn{
                    Landmark.Label,
                    detail::LabelLocationForLandmark(
                        {Landmark, Request.Geometry}),
                    LevelLayoutAdapters::CubeHalfExtent(Request.Geometry) *
                        Request.Geometry.LandmarkLabelWorldSizeScale};
              })};
}

inline FLevelSectionSpawn
BuildNatureSectionSpawn(const FLevelNatureSectionSpawnRequest &Request) {
  return {func::map_array<FNatureFeatureSeed, FLevelBlockSpawn>(
              Request.Features,
              [&Request](const FNatureFeatureSeed &Feature) {
                return FLevelBlockSpawn{
                    Feature.Name,
                    LevelLayoutAdapters::ToWorld(
                        {Request.TerrainData, Feature.Location}),
                    FRotator::ZeroRotator,
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
BuildWorldRoute(const FLevelWorldRouteRequest &Request) {
  return func::map_array<FLevelLocalPoint, FVector>(
      Request.Route, [&Request](const FLevelLocalPoint &Point) {
        return LevelLayoutAdapters::ToWorld({Request.TerrainData, Point});
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

#include "Core/rtk.hpp"

#include "Features/Systems/Level/Actions.h"
#include "Features/Systems/Level/Selectors.h"
#include "Features/Systems/Level/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemSlice {

inline FLevelSystemState CreateInitialState() {
  return (func::pipe(FLevelSystemState{}) |
          [](FLevelSystemState State) -> FLevelSystemState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FLevelSystemState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FLevelSystemState>> Slice =
      func::lazy([]() -> rtk::Slice<FLevelSystemState> {
        // RTK guidance: slice names are reducer/action metadata, not authored gameplay data.
        return rtk::createSlice<FLevelSystemState>(
            TEXT("systems/level"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FLevelSystemState>
                   &Builder) {
              Builder.addCase(LevelSystemActions::LevelObserved(),
                              LevelSystemReducers::ReduceLevelObserved);
            });
      });
  return func::eval(Slice);
}

inline FLevelSectionSpawn
BuildSectionSpawn(const FLevelSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildSectionSpawn(Request);
}

inline FLevelSectionSpawn
BuildOverlaySectionSpawn(const FLevelOverlaySectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildOverlaySectionSpawn(Request);
}

inline FLevelSectionSpawn
BuildLandmarkSectionSpawn(const FLevelLandmarkSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildLandmarkSectionSpawn(Request);
}

inline FLevelSectionSpawn
BuildNatureSectionSpawn(const FLevelNatureSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildNatureSectionSpawn(Request);
}

inline TArray<FVector> BuildWorldRoute(const FLevelWorldRouteRequest &Request) {
  return LevelSystemReducers::BuildWorldRoute(Request);
}

} // namespace LevelSystemSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
