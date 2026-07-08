#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureActions {

const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &NatureSeeded();

} // namespace NatureActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
