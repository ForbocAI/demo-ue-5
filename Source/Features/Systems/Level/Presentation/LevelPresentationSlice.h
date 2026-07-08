#pragma once

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

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
