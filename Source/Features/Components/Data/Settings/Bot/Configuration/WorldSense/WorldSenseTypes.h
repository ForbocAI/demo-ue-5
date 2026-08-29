#pragma once

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotWorldSenseSettings {
  bool bDefaultHazardOverlapping;
  bool bDefaultVisibilityCanSeeEnemy;
};

/** User Story: As a bot settings consumer, I need to compare world-sense defaults so authored observation input remains deterministic. @fn inline bool operator==(const FBotWorldSenseSettings &Left, const FBotWorldSenseSettings &Right) */
inline bool operator==(const FBotWorldSenseSettings &Left,
                       const FBotWorldSenseSettings &Right) {
  return Left.bDefaultHazardOverlapping ==
             Right.bDefaultHazardOverlapping &&
         Left.bDefaultVisibilityCanSeeEnemy ==
             Right.bDefaultVisibilityCanSeeEnemy;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
