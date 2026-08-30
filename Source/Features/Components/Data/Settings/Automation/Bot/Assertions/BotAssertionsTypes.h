#pragma once

#include "CoreMinimal.h"

/** Typed authored labels for the bot automation assertion domains. */

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Bot {

struct FStateAssertions {
  FString Name;
  FString Health;
  FString Phase;
  FString IdValid;
};

struct FMovementAssertions {
  FString PositionX;
};

struct FCombatAssertions {
  FString HealthReduced;
  FString PhaseCombat;
  FString PhaseFlee;
};

struct FAwarenessAssertions {
  FString HasAggro;
  FString TimeSinceSeen;
  FString KnownPlayerPositionX;
  FString PhaseCombat;
};

struct FTickAssertions {
  FString TickCount;
  FString InitiallyAggro;
  FString LostAggro;
};

struct FAssertions {
  FStateAssertions State;
  FMovementAssertions Movement;
  FCombatAssertions Combat;
  FAwarenessAssertions Awareness;
  FTickAssertions Tick;
};

/** User Story: As a bot assertion consumer, I need state assertion equality through a stable signature so authored test labels remain deterministic. @fn inline bool operator==(const FStateAssertions &Left, const FStateAssertions &Right) */
inline bool operator==(const FStateAssertions &Left,
                       const FStateAssertions &Right) {
  return Left.Name == Right.Name && Left.Health == Right.Health &&
         Left.Phase == Right.Phase && Left.IdValid == Right.IdValid;
}

/** User Story: As a bot assertion consumer, I need state assertion inequality through a stable signature so authored test labels remain deterministic. @fn inline bool operator!=(const FStateAssertions &Left, const FStateAssertions &Right) */
inline bool operator!=(const FStateAssertions &Left,
                       const FStateAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a bot assertion consumer, I need movement assertion equality through a stable signature so authored test labels remain deterministic. @fn inline bool operator==(const FMovementAssertions &Left, const FMovementAssertions &Right) */
inline bool operator==(const FMovementAssertions &Left,
                       const FMovementAssertions &Right) {
  return Left.PositionX == Right.PositionX;
}

/** User Story: As a bot assertion consumer, I need movement assertion inequality through a stable signature so authored test labels remain deterministic. @fn inline bool operator!=(const FMovementAssertions &Left, const FMovementAssertions &Right) */
inline bool operator!=(const FMovementAssertions &Left,
                       const FMovementAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a bot assertion consumer, I need combat assertion equality through a stable signature so authored test labels remain deterministic. @fn inline bool operator==(const FCombatAssertions &Left, const FCombatAssertions &Right) */
inline bool operator==(const FCombatAssertions &Left,
                       const FCombatAssertions &Right) {
  return Left.HealthReduced == Right.HealthReduced &&
         Left.PhaseCombat == Right.PhaseCombat &&
         Left.PhaseFlee == Right.PhaseFlee;
}

/** User Story: As a bot assertion consumer, I need combat assertion inequality through a stable signature so authored test labels remain deterministic. @fn inline bool operator!=(const FCombatAssertions &Left, const FCombatAssertions &Right) */
inline bool operator!=(const FCombatAssertions &Left,
                       const FCombatAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a bot assertion consumer, I need awareness assertion equality through a stable signature so authored test labels remain deterministic. @fn inline bool operator==(const FAwarenessAssertions &Left, const FAwarenessAssertions &Right) */
inline bool operator==(const FAwarenessAssertions &Left,
                       const FAwarenessAssertions &Right) {
  return Left.HasAggro == Right.HasAggro &&
         Left.TimeSinceSeen == Right.TimeSinceSeen &&
         Left.KnownPlayerPositionX == Right.KnownPlayerPositionX &&
         Left.PhaseCombat == Right.PhaseCombat;
}

/** User Story: As a bot assertion consumer, I need awareness assertion inequality through a stable signature so authored test labels remain deterministic. @fn inline bool operator!=(const FAwarenessAssertions &Left, const FAwarenessAssertions &Right) */
inline bool operator!=(const FAwarenessAssertions &Left,
                       const FAwarenessAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a bot assertion consumer, I need tick assertion equality through a stable signature so authored test labels remain deterministic. @fn inline bool operator==(const FTickAssertions &Left, const FTickAssertions &Right) */
inline bool operator==(const FTickAssertions &Left,
                       const FTickAssertions &Right) {
  return Left.TickCount == Right.TickCount &&
         Left.InitiallyAggro == Right.InitiallyAggro &&
         Left.LostAggro == Right.LostAggro;
}

/** User Story: As a bot assertion consumer, I need tick assertion inequality through a stable signature so authored test labels remain deterministic. @fn inline bool operator!=(const FTickAssertions &Left, const FTickAssertions &Right) */
inline bool operator!=(const FTickAssertions &Left,
                       const FTickAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a bot assertion consumer, I need aggregate assertion equality through a stable signature so authored test labels remain deterministic. @fn inline bool operator==(const FAssertions &Left, const FAssertions &Right) */
inline bool operator==(const FAssertions &Left, const FAssertions &Right) {
  return Left.State == Right.State && Left.Movement == Right.Movement &&
         Left.Combat == Right.Combat && Left.Awareness == Right.Awareness &&
         Left.Tick == Right.Tick;
}

/** User Story: As a bot assertion consumer, I need aggregate assertion inequality through a stable signature so authored test labels remain deterministic. @fn inline bool operator!=(const FAssertions &Left, const FAssertions &Right) */
inline bool operator!=(const FAssertions &Left, const FAssertions &Right) {
  return !(Left == Right);
}

} // namespace Bot
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
