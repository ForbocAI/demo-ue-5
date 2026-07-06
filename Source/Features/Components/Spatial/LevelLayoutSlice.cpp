#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelLayoutSlice {
namespace {

FLevelLocalPoint PostOfficeGroundPoint(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return Point({Geometry.PostOfficeEastLots * TownLotWorldUnits(Geometry),
                Geometry.PostOfficeNorthLots * TownLotWorldUnits(Geometry),
                0.0f});
}

} // namespace

float TownLotWorldUnits(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return Geometry.TerrainWorldSize / Geometry.TerrainLotsAcross;
}

float CubeHalfExtent(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return Geometry.CubeMeshSize * 0.5f;
}

float BuildingFoundationHeight(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.FoundationHeightRatio;
}

float RoadSurfaceClearance(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.RoadClearanceRatio;
}

float CharacterHeightOffset(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.CharacterHeightRatio;
}

float LabelHeightForScale(const FLevelLabelHeightRequest &Request) {
  return (Request.Scale.Z * Request.Geometry.CubeMeshSize) +
         BuildingFoundationHeight(Request.Geometry) +
         (CubeHalfExtent(Request.Geometry) *
          Request.Geometry.LabelClearanceRatio);
}

float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) {
  return Request.Feet * Request.Geometry.ActorFootToTerrainRatio *
         Request.Geometry.TerrainWorldSize /
         Request.Geometry.ActorReferenceFeetAcross;
}

float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request) {
  return ActorWorldUnitsFromFeet(Request) /
         (CubeHalfExtent(Request.Geometry) * 2.0f);
}

FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request) {
  return FVector(Request.FrontageFeet * Request.Geometry.BlockScalePerFoot,
                 Request.DepthFeet * Request.Geometry.BlockScalePerFoot,
                 Request.Stories * Request.Geometry.HeightScalePerStory);
}

FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request) {
  return FVector(
      Request.WidthFeet * Request.Geometry.BlockScalePerFoot,
      Request.LengthLots * TownLotWorldUnits(Request.Geometry) /
          Request.Geometry.CubeMeshSize,
      Request.HeightFeet * Request.Geometry.BlockScalePerFoot);
}

FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request) {
  return FVector(Request.WidthFeet * Request.Geometry.BlockScalePerFoot,
                 Request.DepthFeet * Request.Geometry.BlockScalePerFoot,
                 Request.HeightFeet * Request.Geometry.BlockScalePerFoot);
}

FLevelLocalPoint Point(const FLevelLayoutPointRequest &Request) {
  return {Request.EastWest, Request.NorthSouth, Request.HeightOffset};
}

FLevelLocalPoint FromPostOfficeLots(const FLevelLayoutLotsRequest &Request) {
  const FLevelLocalPoint PostOffice = PostOfficeGroundPoint(Request.Geometry);
  return Point({PostOffice.EastWest +
                    Request.EastLots * TownLotWorldUnits(Request.Geometry),
                PostOffice.NorthSouth +
                    Request.NorthLots * TownLotWorldUnits(Request.Geometry),
                Request.HeightOffset});
}

FLevelLocalPoint CenteredOnGround(
    const FLevelCenteredOnGroundRequest &Request) {
  return {Request.Point.EastWest, Request.Point.NorthSouth,
          static_cast<float>(Request.Scale.Z *
                                 CubeHalfExtent(Request.Geometry) +
                             Request.GroundClearance)};
}

#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelLayoutSlice {
namespace {

FLevelLocalPoint PostOfficeGroundPoint(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return Point({Geometry.PostOfficeEastLots * TownLotWorldUnits(Geometry),
                Geometry.PostOfficeNorthLots * TownLotWorldUnits(Geometry),
                0.0f});
}

} // namespace

float TownLotWorldUnits(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return Geometry.TerrainWorldSize / Geometry.TerrainLotsAcross;
}

float CubeHalfExtent(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return Geometry.CubeMeshSize * 0.5f;
}

float BuildingFoundationHeight(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.FoundationHeightRatio;
}

float RoadSurfaceClearance(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.RoadClearanceRatio;
}

float CharacterHeightOffset(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.CharacterHeightRatio;
}

float LabelHeightForScale(const FLevelLabelHeightRequest &Request) {
  return (Request.Scale.Z * Request.Geometry.CubeMeshSize) +
         BuildingFoundationHeight(Request.Geometry) +
         (CubeHalfExtent(Request.Geometry) *
          Request.Geometry.LabelClearanceRatio);
}

float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) {
  return Request.Feet * Request.Geometry.ActorFootToTerrainRatio *
         Request.Geometry.TerrainWorldSize /
         Request.Geometry.ActorReferenceFeetAcross;
}

float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request) {
  return ActorWorldUnitsFromFeet(Request) /
         (CubeHalfExtent(Request.Geometry) * 2.0f);
}

FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request) {
  return FVector(Request.FrontageFeet * Request.Geometry.BlockScalePerFoot,
                 Request.DepthFeet * Request.Geometry.BlockScalePerFoot,
                 Request.Stories * Request.Geometry.HeightScalePerStory);
}

FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request) {
  return FVector(
      Request.WidthFeet * Request.Geometry.BlockScalePerFoot,
      Request.LengthLots * TownLotWorldUnits(Request.Geometry) /
          Request.Geometry.CubeMeshSize,
      Request.HeightFeet * Request.Geometry.BlockScalePerFoot);
}

FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request) {
  return FVector(Request.WidthFeet * Request.Geometry.BlockScalePerFoot,
                 Request.DepthFeet * Request.Geometry.BlockScalePerFoot,
                 Request.HeightFeet * Request.Geometry.BlockScalePerFoot);
}

FLevelLocalPoint Point(const FLevelLayoutPointRequest &Request) {
  return {Request.EastWest, Request.NorthSouth, Request.HeightOffset};
}

FLevelLocalPoint FromPostOfficeLots(const FLevelLayoutLotsRequest &Request) {
  const FLevelLocalPoint PostOffice = PostOfficeGroundPoint(Request.Geometry);
  return Point({PostOffice.EastWest +
                    Request.EastLots * TownLotWorldUnits(Request.Geometry),
                PostOffice.NorthSouth +
                    Request.NorthLots * TownLotWorldUnits(Request.Geometry),
                Request.HeightOffset});
}

FLevelLocalPoint CenteredOnGround(
    const FLevelCenteredOnGroundRequest &Request) {
  return {Request.Point.EastWest, Request.Point.NorthSouth,
          static_cast<float>(Request.Scale.Z *
                                 CubeHalfExtent(Request.Geometry) +
                             Request.GroundClearance)};
}

FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request) {
  return {Request.Point.EastWest, Request.Point.NorthSouth,
          LabelHeightForScale({Request.Geometry, Request.Scale})};
}

FVector ToWorld(const FLevelToWorldRequest &Request) {
  return Request.TerrainData.ToWorld(
      FVector2D(Request.Point.EastWest, Request.Point.NorthSouth));
}

FLevelLocalPoint PlayerSpawnPoint(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return FromPostOfficeLots(
      {Geometry, Geometry.PlayerSpawnEastLots, Geometry.PlayerSpawnNorthLots,
       CharacterHeightOffset(Geometry) +
           CubeHalfExtent(Geometry) * Geometry.PlayerSpawnExtraHeightRatio});
}

FRotator PlayerSpawnRotation(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return FRotator(0.0f, Geometry.MainStreetFacingYawDegrees, 0.0f);
}

FString PlayerSpawnAnchorLabel(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return Geometry.PlayerSpawnAnchorLabel;
}

} // namespace LevelLayoutSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
