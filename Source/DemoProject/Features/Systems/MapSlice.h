#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapSlice {

const rtk::Slice<FMapState> &GetSlice();
rtk::EnhancedStore<FMapState> ConfigureStore();

} // namespace MapSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
