#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/Nature/NatureFactories.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureSlice {

const rtk::Slice<FNatureState> &GetSlice();

} // namespace NatureSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
