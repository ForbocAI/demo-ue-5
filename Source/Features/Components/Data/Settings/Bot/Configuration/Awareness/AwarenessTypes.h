#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotAwarenessSettings {
  FVector InitialKnownPlayerPosition;
  float InitialTimeSinceSeenPlayer;
  float EnemySpottedTimeSinceSeenPlayer;
  bool bInitialHasAggro;
  float AggroTimeoutSeconds;
};

/** User Story: As a bot settings consumer, I need to compare awareness settings so authored perception memory remains deterministic. @fn inline bool operator==(const FBotAwarenessSettings &Left, const FBotAwarenessSettings &Right) */
inline bool operator==(const FBotAwarenessSettings &Left,
                       const FBotAwarenessSettings &Right) {
  return Left.InitialKnownPlayerPosition ==
             Right.InitialKnownPlayerPosition &&
         FMath::IsNearlyEqual(Left.InitialTimeSinceSeenPlayer,
                              Right.InitialTimeSinceSeenPlayer) &&
         FMath::IsNearlyEqual(Left.EnemySpottedTimeSinceSeenPlayer,
                              Right.EnemySpottedTimeSinceSeenPlayer) &&
         Left.bInitialHasAggro == Right.bInitialHasAggro &&
         FMath::IsNearlyEqual(Left.AggroTimeoutSeconds,
                              Right.AggroTimeoutSeconds);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
