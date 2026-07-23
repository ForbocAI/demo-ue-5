#pragma once

#include "Features/Systems/Speech/Component/ComponentAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechSelectors {

/** User Story: As a features systems speech consumer, I need to invoke select speech component class through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline TSubclassOf<USpeechComponent> SelectSpeechComponentClass() */
inline TSubclassOf<USpeechComponent> SelectSpeechComponentClass() {
  return USpeechComponent::StaticClass();
}

} // namespace SpeechSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
