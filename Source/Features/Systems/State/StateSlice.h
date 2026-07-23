#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSlice {

/** User Story: As a features systems state consumer, I need to invoke get slice through a stable signature so the features systems state workflow remains explicit and composable. @fn const rtk::Slice<FRuntimeState> &GetSlice() */
const rtk::Slice<FRuntimeState> &GetSlice();
/** User Story: As a features systems state consumer, I need to invoke request player spawn through a stable signature so the features systems state workflow remains explicit and composable. @fn rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn() */
rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn();
/** User Story: As a features systems state consumer, I need to invoke request level view payload through a stable signature so the features systems state workflow remains explicit and composable. @fn rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> RequestLevelViewPayload() */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

} // namespace RuntimeSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
