#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataReducers {

/** User Story: As a features components data consumer, I need to invoke reduce data loaded through a stable signature so the features components data workflow remains explicit and composable. @fn FDataState ReduceDataLoaded(const FDataState &State, const rtk::PayloadAction<FDataLoadedPayload> &Action) */
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

/** User Story: As a features components data consumer, I need to invoke get slice through a stable signature so the features components data workflow remains explicit and composable. @fn const rtk::Slice<FDataState> &GetSlice() */
const rtk::Slice<FDataState> &GetSlice();

} // namespace DataSlice
} // namespace Data
} // namespace Game
} // namespace ForbocAI
