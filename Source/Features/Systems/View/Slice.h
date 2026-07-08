#pragma once

#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

/**
 * @brief Reduces townsperson seed data into spawn data for views.
 */
FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request);

FRuntimeHorseViewSpawn
ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request);

FRuntimeLevelViewPayload ReduceLevelViewPayload(
    const FRuntimeState &State, const FRuntimeLevelViewPayloadRequest &Request);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
