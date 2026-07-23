#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemSelectors {

/** User Story: As a features systems level consumer, I need to invoke select ready through a stable signature so the features systems level workflow remains explicit and composable. @fn inline bool SelectReady(const FSystemState &State) */
inline bool SelectReady(const FSystemState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/** User Story: As a features systems level consumer, I need to invoke select action id through a stable signature so the features systems level workflow remains explicit and composable. @fn inline func::Maybe<FString> SelectActionId(const FSystemState &State) */
inline func::Maybe<FString>
SelectActionId(const FSystemState &State) {
  return State.ActionId;
}

/** User Story: As a features systems level consumer, I need to invoke select terrain sources through a stable signature so the features systems level workflow remains explicit and composable. @fn inline const ForbocAI::Game::Data::FSourceSettings & SelectTerrainSources(const FSystemState &State) */
inline const ForbocAI::Game::Data::FSourceSettings &
SelectTerrainSources(const FSystemState &State) {
  return State.TerrainSources;
}

/** User Story: As a features systems level consumer, I need to invoke select csv through a stable signature so the features systems level workflow remains explicit and composable. @fn inline const ForbocAI::Game::Data::FCsvSettings & SelectCsv(const FSystemState &State) */
inline const ForbocAI::Game::Data::FCsvSettings &
SelectCsv(const FSystemState &State) {
  return State.Csv;
}

/** User Story: As a features systems level consumer, I need to invoke select data sources through a stable signature so the features systems level workflow remains explicit and composable. @fn inline const ForbocAI::Game::Data::FDataSourceSettings & SelectDataSources(const FSystemState &State) */
inline const ForbocAI::Game::Data::FDataSourceSettings &
SelectDataSources(const FSystemState &State) {
  return State.DataSources;
}

/** User Story: As a features systems level consumer, I need to invoke select geometry through a stable signature so the features systems level workflow remains explicit and composable. @fn inline const ForbocAI::Game::Data::FGeometrySettings & SelectGeometry(const FSystemState &State) */
inline const ForbocAI::Game::Data::FGeometrySettings &
SelectGeometry(const FSystemState &State) {
  return State.Geometry;
}

} // namespace LevelSystemSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
