#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSelectors {

TArray<FFeatureSeed> SelectAll(const FNatureState &State);

} // namespace NatureSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
