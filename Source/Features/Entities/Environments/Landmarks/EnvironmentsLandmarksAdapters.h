#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Environments/Landmarks/EnvironmentsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarksAdapters {

struct FLandmarkSeedBuildRequest {
  FString RelativeJsonPath;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

using FProjectLandmarkEntityPayload =
    EntitiesAdapters::TProjectEntityPayload<FLandmark>;

/** User Story: As a landmark entity consumer, I need a stable logical key so landmark systems and views address one ECS entity. @fn ecs::EntityKey LandmarkEntityKey(const FString &Id) */
ecs::EntityKey LandmarkEntityKey(const FString &Id);

/** User Story: As a landmark entity consumer, I need landmark records projected through their domain-owned adapter so the root ECS world remains normalized. @fn ecs::FWorld ProjectLandmark(const FProjectLandmarkEntityPayload &Payload) */
ecs::FWorld ProjectLandmark(const FProjectLandmarkEntityPayload &Payload);

/** User Story: As a entities environments landmarks consumer, I need to invoke build landmark seed through a stable signature so the entities environments landmarks workflow remains explicit and composable. @fn TArray<FLandmark> BuildLandmarkSeed( const FLandmarkSeedBuildRequest &Request) */
TArray<FLandmark> BuildLandmarkSeed(
    const FLandmarkSeedBuildRequest &Request);

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
