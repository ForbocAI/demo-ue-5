#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

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

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataActions.h"
#include "Features/Components/Data/DataSelectors.h"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataSlice {

const rtk::Slice<FDataState> &GetSlice();

} // namespace DataSlice
} // namespace Data
} // namespace Game
} // namespace ForbocAI
