#pragma once

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotLifecycleSettings {
  bool bOrchestratorCanEverTick;
  bool bRegisteredBotActive;
};

/** User Story: As a bot settings consumer, I need to compare lifecycle settings so authored runtime activation remains deterministic. @fn inline bool operator==(const FBotLifecycleSettings &Left, const FBotLifecycleSettings &Right) */
inline bool operator==(const FBotLifecycleSettings &Left,
                       const FBotLifecycleSettings &Right) {
  return Left.bOrchestratorCanEverTick == Right.bOrchestratorCanEverTick &&
         Left.bRegisteredBotActive == Right.bRegisteredBotActive;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
