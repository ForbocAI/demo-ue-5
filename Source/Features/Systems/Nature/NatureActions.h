#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureActions {

/** User Story: As a features systems nature consumer, I need to invoke nature seeded through a stable signature so the features systems nature workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FFeatureSeed>> &NatureSeeded() */
const rtk::ActionCreator<TArray<FFeatureSeed>> &NatureSeeded();

} // namespace NatureActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
