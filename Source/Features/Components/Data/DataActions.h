#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataActions {

const rtk::ActionCreator<FDataLoadedPayload> &DataLoaded();

} // namespace DataActions
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
