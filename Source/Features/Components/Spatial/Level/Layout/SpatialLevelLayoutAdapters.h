#pragma once

#include "Features/Components/Spatial/Level/Layout/LayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace LevelLayoutAdapters {
float TownLotWorldUnits(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
float CubeHalfExtent(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
float BuildingFoundationHeight(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
float RoadSurfaceClearance(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
float CharacterHeightOffset(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
float LabelHeightForScale(const FLevelLabelHeightRequest &Request);
float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request);
float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request);

FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request);
FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request);
FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request);

FLevelLocalPoint Point(const FPointRequest &Request);
FLevelLocalPoint FromPostOfficeLots(const FLotsRequest &Request);
FLevelLocalPoint CenteredOnGround(const FLevelCenteredOnGroundRequest &Request);
FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request);
FVector ToWorld(const FLevelToWorldRequest &Request);

FLevelLocalPoint PlayerSpawnPoint(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
FRotator PlayerSpawnRotation(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
FString PlayerSpawnAnchorLabel(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry);
} // namespace LevelLayoutAdapters

} // namespace Level
} // namespace Game
} // namespace ForbocAI
