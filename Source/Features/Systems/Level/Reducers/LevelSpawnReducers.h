#pragma once

#include "Features/Systems/Level/Reducers/LevelGeometryReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemReducers {
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
} // namespace LevelSystemReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
