#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

/** User Story: As a systems nature entity consumer, I need to invoke nature adapter through a stable signature so the systems nature entity workflow remains explicit and composable. @fn const rtk::EntityAdapter<FFeatureSeed> &NatureAdapter() */
const rtk::EntityAdapter<FFeatureSeed> &NatureAdapter();

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
