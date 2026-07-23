#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureReducers {

/** User Story: As a features systems nature consumer, I need to invoke reduce nature seeded through a stable signature so the features systems nature workflow remains explicit and composable. @fn FNatureState ReduceNatureSeeded( const FNatureState &State, const rtk::PayloadAction<TArray<FFeatureSeed>> &Action) */
FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::PayloadAction<TArray<FFeatureSeed>> &Action);

} // namespace NatureReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/SystemsNatureActions.h"
#include "Features/Systems/Nature/SystemsNatureSelectors.h"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSlice {

/** User Story: As a features systems nature consumer, I need to invoke get slice through a stable signature so the features systems nature workflow remains explicit and composable. @fn const rtk::Slice<FNatureState> &GetSlice() */
const rtk::Slice<FNatureState> &GetSlice();

} // namespace NatureSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureFactories {

/** User Story: As a features systems nature consumer, I need to invoke create initial state through a stable signature so the features systems nature workflow remains explicit and composable. @fn FNatureState CreateInitialState() */
FNatureState CreateInitialState();

} // namespace NatureFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
