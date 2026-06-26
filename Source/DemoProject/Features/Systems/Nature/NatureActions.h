#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureActions {

const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &NatureSeeded();

} // namespace NatureActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
