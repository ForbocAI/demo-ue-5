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

/** User Story: As a systems level geometry consumer, I need to invoke ground lots through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FLevelLocalPoint GroundLots(const FGroundLotsRequest &Request) */
inline FLevelLocalPoint GroundLots(const FGroundLotsRequest &Request) {
  return LevelLayoutAdapters::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutAdapters::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
       Request.Scale, Request.Clearance});
}

/** User Story: As a systems level geometry consumer, I need to invoke building lots through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FLevelLocalPoint BuildingLots(const FGroundLotsRequest &Request) */
inline FLevelLocalPoint BuildingLots(const FGroundLotsRequest &Request) {
  return GroundLots({Request.Geometry, Request.EastLots, Request.NorthLots,
                     Request.Scale,
                     LevelLayoutAdapters::BuildingFoundationHeight(
                         Request.Geometry)});
}

/** User Story: As a systems level geometry consumer, I need to invoke feature lots through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FLevelLocalPoint FeatureLots(const FGroundLotsRequest &Request) */
inline FLevelLocalPoint FeatureLots(const FGroundLotsRequest &Request) {
  return GroundLots({Request.Geometry, Request.EastLots, Request.NorthLots,
                     Request.Scale,
                     LevelLayoutAdapters::RoadSurfaceClearance(Request.Geometry)});
}

/** User Story: As a systems level geometry consumer, I need to invoke scale from seed through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FVector ScaleFromSeed(const FScaleFromSeedRequest &Request) */
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

/** User Story: As a systems level geometry consumer, I need to invoke local point from seed through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FLevelLocalPoint LocalPointFromSeed( const FLocalPointFromSeedRequest &Request) */
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

/** User Story: As a systems level geometry consumer, I need to invoke world location from seed through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FVector WorldLocationFromSeed( const FWorldLocationFromSeedRequest &Request) */
inline FVector WorldLocationFromSeed(
    const FWorldLocationFromSeedRequest &Request) {
  return Request.Seed.Anchor == EAnchorMode::World
             ? Request.Seed.WorldLocation
             : LevelLayoutAdapters::ToWorld(
                   {Request.TerrainData,
                    LocalPointFromSeed({Request.Seed, Request.Geometry,
                                        Request.Scale})});
}

/** User Story: As a systems level geometry consumer, I need to invoke label height offset from seed through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline float LabelHeightOffsetFromSeed( const FLabelHeightOffsetRequest &Request) */
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

/** User Story: As a systems level geometry consumer, I need to invoke explicit label local point from seed through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FLevelLocalPoint ExplicitLabelLocalPointFromSeed( const FLabelLocalPointRequest &Request) */
inline FLevelLocalPoint ExplicitLabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  return LevelLayoutAdapters::FromPostOfficeLots(
      {Request.Geometry, Request.Seed.EastLots, Request.Seed.NorthLots,
       Request.Seed.HeightOffset});
}

/** User Story: As a systems level geometry consumer, I need to invoke reference label local point from seed through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FLevelLocalPoint ReferenceLabelLocalPointFromSeed( const FLabelLocalPointRequest &Request) */
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

/** User Story: As a systems level geometry consumer, I need to invoke label local point from seed through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FLevelLocalPoint LabelLocalPointFromSeed( const FLabelLocalPointRequest &Request) */
inline FLevelLocalPoint LabelLocalPointFromSeed(
    const FLabelLocalPointRequest &Request) {
  return Request.Seed.Height == ELabelHeightMode::Explicit
             ? ExplicitLabelLocalPointFromSeed(Request)
             : ReferenceLabelLocalPointFromSeed(Request);
}

/** User Story: As a systems level geometry consumer, I need to invoke label location for landmark through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline FVector LabelLocationForLandmark( const FLandmarkLabelLocationRequest &Request) */
inline FVector LabelLocationForLandmark(
    const FLandmarkLabelLocationRequest &Request) {
  return Request.Landmark.Location +
         FVector::UpVector *
             (LevelLayoutAdapters::CubeHalfExtent(Request.Geometry) *
                  Request.Landmark.Scale.Z +
              LevelLayoutAdapters::BuildingFoundationHeight(Request.Geometry) +
              LevelLayoutAdapters::CubeHalfExtent(Request.Geometry));
}

/** User Story: As a systems level geometry consumer, I need to invoke texture for nature kind through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline ELevelRetroTexture TextureForNatureKind(EFeatureKind Kind) */
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

/** User Story: As a systems level geometry consumer, I need to invoke nature feature needs label through a stable signature so the systems level geometry workflow remains explicit and composable. @fn inline bool NatureFeatureNeedsLabel(EFeatureKind Kind) */
inline bool NatureFeatureNeedsLabel(EFeatureKind Kind) {
  return Kind == EFeatureKind::PCGMarker ||
         Kind == EFeatureKind::WaterSystemMarker;
}

} // namespace detail
} // namespace LevelSystemReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
