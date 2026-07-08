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

const rtk::Slice<FRuntimeState> &GetSlice();
rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn();
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

} // namespace RuntimeSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
