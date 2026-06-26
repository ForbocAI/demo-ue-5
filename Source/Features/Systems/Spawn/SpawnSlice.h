#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Spawn/SpawnSelectors.h"
#include "Features/Systems/Spawn/SpawnTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnSlice {

const rtk::Slice<FSpawnState> &GetSlice();

} // namespace SpawnSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
