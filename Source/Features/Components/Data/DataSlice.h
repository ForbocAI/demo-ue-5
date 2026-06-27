#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataActions.h"
#include "Features/Components/Data/DataReducers.h"
#include "Features/Components/Data/DataSelectors.h"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataSlice {

const rtk::Slice<FDataState> &GetSlice();

} // namespace DataSlice
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
