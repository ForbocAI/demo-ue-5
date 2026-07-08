#pragma once

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace LevelLayoutAdapters {
float TownLotWorldUnits(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
float CubeHalfExtent(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
float BuildingFoundationHeight(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
float RoadSurfaceClearance(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
float CharacterHeightOffset(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
float LabelHeightForScale(const FLevelLabelHeightRequest &Request);
float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request);
float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request);

FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request);
FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request);
FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request);

FLevelLocalPoint Point(const FLevelLayoutPointRequest &Request);
FLevelLocalPoint FromPostOfficeLots(const FLevelLayoutLotsRequest &Request);
FLevelLocalPoint CenteredOnGround(const FLevelCenteredOnGroundRequest &Request);
FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request);
FVector ToWorld(const FLevelToWorldRequest &Request);

FLevelLocalPoint PlayerSpawnPoint(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
FRotator PlayerSpawnRotation(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
FString PlayerSpawnAnchorLabel(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
} // namespace LevelLayoutAdapters

} // namespace Level
} // namespace Game
} // namespace ForbocAI
