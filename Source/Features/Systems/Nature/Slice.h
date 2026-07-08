#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/Types.h"

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

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/Actions.h"
#include "Features/Systems/Nature/Selectors.h"
#include "Features/Systems/Nature/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSlice {

const rtk::Slice<FNatureState> &GetSlice();

} // namespace NatureSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Nature/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureFactories {

FNatureState CreateInitialState();

} // namespace NatureFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
