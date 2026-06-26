#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureReducers {

FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::Action<TArray<FNatureFeatureSeed>> &Action);

} // namespace NatureReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
