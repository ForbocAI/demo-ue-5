#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataReducers {

FDataState
ReduceDataLoaded(const FDataState &State,
                 const rtk::PayloadAction<FDataLoadedPayload> &Action);

} // namespace DataReducers
} // namespace Data
} // namespace Game
} // namespace ForbocAI
