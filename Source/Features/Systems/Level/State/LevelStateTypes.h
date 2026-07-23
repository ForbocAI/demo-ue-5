#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Level/Geometry/GeometryTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FSystemPayload {
  FString Id;
};

struct FSystemState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  ForbocAI::Game::Data::FSourceSettings TerrainSources;
  ForbocAI::Game::Data::FCsvSettings Csv;
  ForbocAI::Game::Data::FDataSourceSettings DataSources;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  bool bReady = false;
};

/** User Story: As a level system state consumer, I need to compare payloads through a stable signature so reducer inputs remain deterministic. @fn inline bool operator==(const FSystemPayload &Left, const FSystemPayload &Right) */
inline bool operator==(const FSystemPayload &Left,
                       const FSystemPayload &Right) {
  return Left.Id == Right.Id;
}

/** User Story: As a level system state consumer, I need to compare payloads for inequality through a stable signature so reducer input changes remain observable. @fn inline bool operator!=(const FSystemPayload &Left, const FSystemPayload &Right) */
inline bool operator!=(const FSystemPayload &Left,
                       const FSystemPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a level system state consumer, I need to compare state through a stable signature so selector and reducer behavior remains deterministic. @fn inline bool operator==(const FSystemState &Left, const FSystemState &Right) */
inline bool operator==(const FSystemState &Left,
                       const FSystemState &Right) {
  return Left.bReady == Right.bReady &&
         Left.TerrainSources == Right.TerrainSources &&
         Left.Csv == Right.Csv &&
         Left.DataSources == Right.DataSources &&
         Left.Geometry == Right.Geometry &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

/** User Story: As a level system state consumer, I need to compare state for inequality through a stable signature so selector and reducer changes remain observable. @fn inline bool operator!=(const FSystemState &Left, const FSystemState &Right) */
inline bool operator!=(const FSystemState &Left,
                       const FSystemState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
