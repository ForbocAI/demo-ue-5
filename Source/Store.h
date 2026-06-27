#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace Store {

rtk::EnhancedStore<FRuntimeState> ConfigureStore();
rtk::EnhancedStore<FRuntimeState> &GetStore();

} // namespace Store
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
