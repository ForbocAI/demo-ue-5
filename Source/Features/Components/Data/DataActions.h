#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataActions {

/** User Story: As a features components data consumer, I need to invoke data loaded through a stable signature so the features components data workflow remains explicit and composable. @fn const rtk::ActionCreator<FDataLoadedPayload> &DataLoaded() */
const rtk::ActionCreator<FDataLoadedPayload> &DataLoaded();

} // namespace DataActions
} // namespace Data
} // namespace Game
} // namespace ForbocAI
