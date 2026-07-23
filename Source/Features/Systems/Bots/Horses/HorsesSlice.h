#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseReducers {

/** User Story: As a systems bots horses consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots horses workflow remains explicit and composable. @fn FHorseState ReduceHorsesSeeded( const FHorseState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
FHorseState ReduceHorsesSeeded(
    const FHorseState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace HorseReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Horses/HorsesSelectors.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSlice {

/** User Story: As a systems bots horses consumer, I need to invoke get slice through a stable signature so the systems bots horses workflow remains explicit and composable. @fn const rtk::Slice<FHorseState> &GetSlice() */
const rtk::Slice<FHorseState> &GetSlice();

} // namespace HorseSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Horses/HorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseFactories {

/** User Story: As a systems bots horses consumer, I need to invoke create initial state through a stable signature so the systems bots horses workflow remains explicit and composable. @fn FHorseState CreateInitialState() */
FHorseState CreateInitialState();

} // namespace HorseFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
