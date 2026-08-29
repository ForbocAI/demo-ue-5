#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelLayoutAdapters {
namespace {

/** User Story: As a spatial level layout consumer, I need to invoke post office ground point through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint PostOfficeGroundPoint( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FLevelLocalPoint PostOfficeGroundPoint(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return Point({Geometry.TownLayout.PostOfficeEastLots * TownLotWorldUnits(Geometry),
                Geometry.TownLayout.PostOfficeNorthLots * TownLotWorldUnits(Geometry),
                FORBOCAI_DEMOUE5_AUTHORED_NUMBERV75F40683FBFF});
}

} // namespace

/** User Story: As a spatial level layout consumer, I need to invoke town lot world units through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float TownLotWorldUnits( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float TownLotWorldUnits(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return Geometry.TerrainExtent.TerrainWorldSize / Geometry.TerrainExtent.TerrainLotsAcross;
}

/** User Story: As a spatial level layout consumer, I need to invoke cube half extent through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float CubeHalfExtent( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float CubeHalfExtent(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return Geometry.WorldScale.CubeMeshSize * FORBOCAI_DEMOUE5_AUTHORED_NUMBERVD72733281456;
}

/** User Story: As a spatial level layout consumer, I need to invoke building foundation height through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float BuildingFoundationHeight( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float BuildingFoundationHeight(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.TownLayout.FoundationHeightRatio;
}

/** User Story: As a spatial level layout consumer, I need to invoke road surface clearance through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float RoadSurfaceClearance( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float RoadSurfaceClearance(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.TownLayout.RoadClearanceRatio;
}

/** User Story: As a spatial level layout consumer, I need to invoke character height offset through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float CharacterHeightOffset( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float CharacterHeightOffset(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return CubeHalfExtent(Geometry) * Geometry.WorldScale.CharacterHeightRatio;
}

/** User Story: As a spatial level layout consumer, I need to invoke label height for scale through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float LabelHeightForScale(const FLevelLabelHeightRequest &Request) */
float LabelHeightForScale(const FLevelLabelHeightRequest &Request) {
  return (Request.Scale.Z * Request.Geometry.WorldScale.CubeMeshSize) +
         BuildingFoundationHeight(Request.Geometry) +
         (CubeHalfExtent(Request.Geometry) *
          Request.Geometry.WorldScale.LabelClearanceRatio);
}

/** User Story: As a spatial level layout consumer, I need to invoke actor world units from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) */
float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) {
  return Request.Feet * Request.Geometry.WorldScale.ActorFootToTerrainRatio *
         Request.Geometry.TerrainExtent.TerrainWorldSize /
         Request.Geometry.WorldScale.ActorReferenceFeetAcross;
}

/** User Story: As a spatial level layout consumer, I need to invoke actor mesh scale from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request) */
float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request) {
  return ActorWorldUnitsFromFeet(Request) /
         (CubeHalfExtent(Request.Geometry) * FORBOCAI_DEMOUE5_AUTHORED_NUMBERV3681B4242750);
}

/** User Story: As a spatial level layout consumer, I need to invoke building scale from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request) */
FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request) {
  return FVector(Request.FrontageFeet * Request.Geometry.TownLayout.BlockScalePerFoot,
                 Request.DepthFeet * Request.Geometry.TownLayout.BlockScalePerFoot,
                 Request.Stories * Request.Geometry.TownLayout.HeightScalePerStory);
}

/** User Story: As a spatial level layout consumer, I need to invoke long feature scale through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request) */
FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request) {
  return FVector(
      Request.WidthFeet * Request.Geometry.TownLayout.BlockScalePerFoot,
      Request.LengthLots * TownLotWorldUnits(Request.Geometry) /
          Request.Geometry.WorldScale.CubeMeshSize,
      Request.HeightFeet * Request.Geometry.TownLayout.BlockScalePerFoot);
}

/** User Story: As a spatial level layout consumer, I need to invoke pad scale from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request) */
FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request) {
  return FVector(Request.WidthFeet * Request.Geometry.TownLayout.BlockScalePerFoot,
                 Request.DepthFeet * Request.Geometry.TownLayout.BlockScalePerFoot,
                 Request.HeightFeet * Request.Geometry.TownLayout.BlockScalePerFoot);
}

/** User Story: As a spatial level layout consumer, I need to invoke point through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint Point(const FPointRequest &Request) */
FLevelLocalPoint Point(const FPointRequest &Request) {
  return {Request.EastWest, Request.NorthSouth, Request.HeightOffset};
}

/** User Story: As a spatial level layout consumer, I need to invoke from post office lots through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint FromPostOfficeLots(const FLotsRequest &Request) */
FLevelLocalPoint FromPostOfficeLots(const FLotsRequest &Request) {
  const FLevelLocalPoint PostOffice = PostOfficeGroundPoint(Request.Geometry);
  return Point({PostOffice.EastWest +
                    Request.EastLots * TownLotWorldUnits(Request.Geometry),
                PostOffice.NorthSouth +
                    Request.NorthLots * TownLotWorldUnits(Request.Geometry),
                Request.HeightOffset});
}

/** User Story: As a spatial level layout consumer, I need to invoke centered on ground through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint CenteredOnGround( const FLevelCenteredOnGroundRequest &Request) */
FLevelLocalPoint CenteredOnGround(
    const FLevelCenteredOnGroundRequest &Request) {
  return {Request.Point.EastWest, Request.Point.NorthSouth,
          static_cast<float>(Request.Scale.Z *
                                 CubeHalfExtent(Request.Geometry) +
                             Request.GroundClearance)};
}

/** User Story: As a spatial level layout consumer, I need to invoke above block through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request) */
FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request) {
  return {Request.Point.EastWest, Request.Point.NorthSouth,
          LabelHeightForScale({Request.Geometry, Request.Scale})};
}

/** User Story: As a spatial level layout consumer, I need to invoke to world through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector ToWorld(const FLevelToWorldRequest &Request) */
FVector ToWorld(const FLevelToWorldRequest &Request) {
  return Request.TerrainData.ToWorld(
      FVector2D(Request.Point.EastWest, Request.Point.NorthSouth),
      Request.Point.HeightOffset);
}

/** User Story: As a spatial level layout consumer, I need to invoke player spawn point through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint PlayerSpawnPoint( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FLevelLocalPoint PlayerSpawnPoint(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return FromPostOfficeLots(
      {Geometry, Geometry.PlayerSpawn.PlayerSpawnEastLots, Geometry.PlayerSpawn.PlayerSpawnNorthLots,
       CharacterHeightOffset(Geometry) +
           CubeHalfExtent(Geometry) * Geometry.PlayerSpawn.PlayerSpawnExtraHeightRatio});
}

/** User Story: As a spatial level layout consumer, I need to invoke player spawn rotation through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FRotator PlayerSpawnRotation( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FRotator PlayerSpawnRotation(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return FRotator(FORBOCAI_DEMOUE5_AUTHORED_NUMBERV75F40683FBFF, Geometry.PlayerSpawn.MainStreetFacingYawDegrees, FORBOCAI_DEMOUE5_AUTHORED_NUMBERV75F40683FBFF);
}

/** User Story: As a spatial level layout consumer, I need to invoke player spawn anchor label through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FString PlayerSpawnAnchorLabel( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FString PlayerSpawnAnchorLabel(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return Geometry.PlayerSpawn.PlayerSpawnAnchorLabel;
}

} // namespace LevelLayoutAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
