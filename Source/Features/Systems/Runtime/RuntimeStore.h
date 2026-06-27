#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeStore {

const rtk::Slice<FRuntimeState> &GetSlice();
rtk::EnhancedStore<FRuntimeState> ConfigureStore();

} // namespace RuntimeStore
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
