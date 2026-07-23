#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

/** User Story: As a features systems bot consumer, I need to invoke reduce runtime townspeople seeded through a stable signature so the features systems bot workflow remains explicit and composable. @fn FRuntimeState ReduceRuntimeTownspeopleSeeded( const FRuntimeState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FRuntimeState ReduceRuntimeTownspeopleSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);

/** User Story: As a features systems bot consumer, I need to invoke reduce runtime horses seeded through a stable signature so the features systems bot workflow remains explicit and composable. @fn FRuntimeState ReduceRuntimeHorsesSeeded( const FRuntimeState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
FRuntimeState ReduceRuntimeHorsesSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
