#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotProjectionSettings {
  bool bPositionPayloadHasLocalLocation;
  bool bPositionPayloadHasWorldLocation;
  FString StateObservationFormat;
};

/** User Story: As a bot settings consumer, I need to compare projection settings so authored runtime payloads remain deterministic. @fn inline bool operator==(const FBotProjectionSettings &Left, const FBotProjectionSettings &Right) */
inline bool operator==(const FBotProjectionSettings &Left,
                       const FBotProjectionSettings &Right) {
  return Left.bPositionPayloadHasLocalLocation ==
             Right.bPositionPayloadHasLocalLocation &&
         Left.bPositionPayloadHasWorldLocation ==
             Right.bPositionPayloadHasWorldLocation &&
         Left.StateObservationFormat == Right.StateObservationFormat;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
