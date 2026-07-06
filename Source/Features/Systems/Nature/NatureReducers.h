#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureReducers {

FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::PayloadAction<TArray<FNatureFeatureSeed>> &Action);

} // namespace NatureReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
