#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureActions {

const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &NatureSeeded();

} // namespace NatureActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
