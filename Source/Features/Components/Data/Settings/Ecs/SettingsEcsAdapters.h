#pragma once

#include "Core/ecs.hpp"
#include "Features/Components/Data/Settings/Ecs/EcsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace SettingsAdapters {

/** User Story: As ECS initialization, I need every authored domain registration composed into one immutable registry graph. @fn ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings) */
ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings);

} // namespace SettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
