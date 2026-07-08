#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseReducers {

FHorseState ReduceHorsesSeeded(
    const FHorseState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace HorseReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/Actions.h"
#include "Features/Systems/Bots/Horses/Selectors.h"
#include "Features/Systems/Bots/Horses/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSlice {

const rtk::Slice<FHorseState> &GetSlice();

} // namespace HorseSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Horses/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseFactories {

FHorseState CreateInitialState();

} // namespace HorseFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
