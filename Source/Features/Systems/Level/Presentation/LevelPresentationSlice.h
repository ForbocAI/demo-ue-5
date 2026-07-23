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
 * @fn FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn( const FRuntimeTownspersonViewSpawnRequest &Request)
 * @brief Reduces townsperson seed data into spawn data for views.
 * User Story: As a systems level presentation consumer, I need to invoke reduce townsperson view spawn through a stable signature so the systems level presentation workflow remains explicit and composable.
 */
FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request);

/** User Story: As a systems level presentation consumer, I need to invoke reduce horse view spawn through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FRuntimeHorseViewSpawn ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request) */
FRuntimeHorseViewSpawn
ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request);

/** User Story: As a systems level presentation consumer, I need to invoke reduce level view payload through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FRuntimeLevelViewPayload ReduceLevelViewPayload( const FRuntimeState &State, const FRuntimeLevelViewPayloadRequest &Request) */
FRuntimeLevelViewPayload ReduceLevelViewPayload(
    const FRuntimeState &State, const FRuntimeLevelViewPayloadRequest &Request);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
