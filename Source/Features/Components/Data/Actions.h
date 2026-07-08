#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataActions {

const rtk::ActionCreator<FDataLoadedPayload> &DataLoaded();

} // namespace DataActions
} // namespace Data
} // namespace Game
} // namespace ForbocAI
