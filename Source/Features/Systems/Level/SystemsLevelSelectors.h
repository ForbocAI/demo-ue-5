#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemSelectors {

inline bool SelectReady(const FSystemState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectActionId(const FSystemState &State) {
  return State.ActionId;
}

inline const ForbocAI::Game::Data::FTerrainSourceSettings &
SelectTerrainSources(const FSystemState &State) {
  return State.TerrainSources;
}

inline const ForbocAI::Game::Data::FCsvSettings &
SelectCsv(const FSystemState &State) {
  return State.Csv;
}

inline const ForbocAI::Game::Data::FDataSourceSettings &
SelectDataSources(const FSystemState &State) {
  return State.DataSources;
}

inline const ForbocAI::Game::Data::FGeometrySettings &
SelectGeometry(const FSystemState &State) {
  return State.Geometry;
}

} // namespace LevelSystemSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
