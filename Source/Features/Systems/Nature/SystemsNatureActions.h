#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureActions {

const rtk::ActionCreator<TArray<FFeatureSeed>> &NatureSeeded();

} // namespace NatureActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
