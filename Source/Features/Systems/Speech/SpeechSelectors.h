#pragma once

#include "Features/Systems/Speech/SpeechAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechSelectors {

inline TSubclassOf<USpeechComponent> SelectSpeechComponentClass() {
  return USpeechComponent::StaticClass();
}

} // namespace SpeechSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
