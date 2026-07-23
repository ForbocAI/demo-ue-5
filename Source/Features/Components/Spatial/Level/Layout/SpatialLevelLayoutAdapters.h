#pragma once

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace LevelLayoutAdapters {
/** User Story: As a spatial level layout consumer, I need to invoke town lot world units through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float TownLotWorldUnits( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float TownLotWorldUnits(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
/** User Story: As a spatial level layout consumer, I need to invoke cube half extent through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float CubeHalfExtent( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float CubeHalfExtent(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
/** User Story: As a spatial level layout consumer, I need to invoke building foundation height through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float BuildingFoundationHeight( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float BuildingFoundationHeight(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
/** User Story: As a spatial level layout consumer, I need to invoke road surface clearance through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float RoadSurfaceClearance( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float RoadSurfaceClearance(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
/** User Story: As a spatial level layout consumer, I need to invoke character height offset through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float CharacterHeightOffset( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float CharacterHeightOffset(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
/** User Story: As a spatial level layout consumer, I need to invoke label height for scale through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float LabelHeightForScale(const FLevelLabelHeightRequest &Request) */
float LabelHeightForScale(const FLevelLabelHeightRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke actor world units from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) */
float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke actor mesh scale from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request) */
float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request);

/** User Story: As a spatial level layout consumer, I need to invoke building scale from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request) */
FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke long feature scale through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request) */
FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke pad scale from feet through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request) */
FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request);

/** User Story: As a spatial level layout consumer, I need to invoke point through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint Point(const FPointRequest &Request) */
FLevelLocalPoint Point(const FPointRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke from post office lots through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint FromPostOfficeLots(const FLotsRequest &Request) */
FLevelLocalPoint FromPostOfficeLots(const FLotsRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke centered on ground through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint CenteredOnGround(const FLevelCenteredOnGroundRequest &Request) */
FLevelLocalPoint CenteredOnGround(const FLevelCenteredOnGroundRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke above block through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request) */
FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request);
/** User Story: As a spatial level layout consumer, I need to invoke to world through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FVector ToWorld(const FLevelToWorldRequest &Request) */
FVector ToWorld(const FLevelToWorldRequest &Request);

/** User Story: As a spatial level layout consumer, I need to invoke player spawn point through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FLevelLocalPoint PlayerSpawnPoint( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FLevelLocalPoint PlayerSpawnPoint(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
/** User Story: As a spatial level layout consumer, I need to invoke player spawn rotation through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FRotator PlayerSpawnRotation( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FRotator PlayerSpawnRotation(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
/** User Story: As a spatial level layout consumer, I need to invoke player spawn anchor label through a stable signature so the spatial level layout workflow remains explicit and composable. @fn FString PlayerSpawnAnchorLabel( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FString PlayerSpawnAnchorLabel(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
} // namespace LevelLayoutAdapters

} // namespace Level
} // namespace Game
} // namespace ForbocAI
