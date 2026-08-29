#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotSpawnSettings {
  FString InitialName;
  FVector InitialPosition;
  FRotator InitialRotation;
  int32 InitialPhase;
  int32 InitialTickCount;
};

/** User Story: As a bot settings consumer, I need to compare spawn settings so authored bot initialization remains deterministic. @fn inline bool operator==(const FBotSpawnSettings &Left, const FBotSpawnSettings &Right) */
inline bool operator==(const FBotSpawnSettings &Left,
                       const FBotSpawnSettings &Right) {
  return Left.InitialName == Right.InitialName &&
         Left.InitialPosition == Right.InitialPosition &&
         Left.InitialRotation == Right.InitialRotation &&
         Left.InitialPhase == Right.InitialPhase &&
         Left.InitialTickCount == Right.InitialTickCount;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
