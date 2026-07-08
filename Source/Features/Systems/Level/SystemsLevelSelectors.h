#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemSelectors {

inline bool SelectReady(const FLevelSystemState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectActionId(const FLevelSystemState &State) {
  return State.ActionId;
}

inline const ForbocAI::Game::Data::FLevelTerrainSourceSettings &
SelectTerrainSources(const FLevelSystemState &State) {
  return State.TerrainSources;
}

inline const ForbocAI::Game::Data::FLevelDataSourceSettings &
SelectDataSources(const FLevelSystemState &State) {
  return State.DataSources;
}

inline const ForbocAI::Game::Data::FLevelGeometrySettings &
SelectGeometry(const FLevelSystemState &State) {
  return State.Geometry;
}

} // namespace LevelSystemSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
