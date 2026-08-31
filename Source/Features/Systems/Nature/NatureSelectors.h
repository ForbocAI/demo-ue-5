#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSelectors {

/** User Story: As a features systems nature consumer, I need to invoke select all through a stable signature so the features systems nature workflow remains explicit and composable. @fn TArray<FFeatureSeed> SelectAll(const FNatureState &State) */
TArray<FFeatureSeed> SelectAll(const FNatureState &State);

} // namespace NatureSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
