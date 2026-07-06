#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSelectors {

TArray<FNatureFeatureSeed> SelectAll(const FNatureState &State);

} // namespace NatureSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
