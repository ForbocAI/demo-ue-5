#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace RuntimeStore {

const rtk::Slice<FRuntimeState> &GetSlice();
rtk::EnhancedStore<FRuntimeState> ConfigureStore();

} // namespace RuntimeStore
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
