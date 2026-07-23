#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeThunks {

/**
 * @fn FString RequestPlayerSpawnTypePrefix()
 * @brief createAsyncThunk type prefixes for the runtime bootstrap workflows.
 * Shared so reducers/listeners can react to the pending/fulfilled/rejected
 * lifecycle actions without duplicating the prefix strings.
 * User Story: As a features systems consumer, I need to invoke request player spawn type prefix through a stable signature so the features systems workflow remains explicit and composable.
 */
FString RequestPlayerSpawnTypePrefix();
/** User Story: As a features systems consumer, I need to invoke request level view payload type prefix through a stable signature so the features systems workflow remains explicit and composable. @fn FString RequestLevelViewPayloadTypePrefix() */
FString RequestLevelViewPayloadTypePrefix();

/**
 * @fn const rtk::AsyncThunkConfig<FPointPayload, rtk::FEmptyPayload, FRuntimeState> & RequestPlayerSpawnAsyncThunk()
 * @brief The runtime bootstrap createAsyncThunk configs, exposed so slices can
 * react to their pending/fulfilled/rejected lifecycle action creators via
 * builder.addCase (the canonical RTK extraReducers pattern).
 * User Story: As a features systems consumer, I need to invoke request player spawn async thunk through a stable signature so the features systems workflow remains explicit and composable.
 */
const rtk::AsyncThunkConfig<FPointPayload, rtk::FEmptyPayload,
                            FRuntimeState> &
RequestPlayerSpawnAsyncThunk();
/** User Story: As a features systems consumer, I need to invoke request level view payload async thunk through a stable signature so the features systems workflow remains explicit and composable. @fn const rtk::AsyncThunkConfig<FRuntimeLevelViewPayload, rtk::FEmptyPayload, FRuntimeState> & RequestLevelViewPayloadAsyncThunk() */
const rtk::AsyncThunkConfig<FRuntimeLevelViewPayload, rtk::FEmptyPayload,
                            FRuntimeState> &
RequestLevelViewPayloadAsyncThunk();

/**
 * @fn rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn()
 * @brief RTK thunk that loads runtime data and resolves the player spawn.
 * User Story: As a features systems consumer, I need to invoke request player spawn through a stable signature so the features systems workflow remains explicit and composable.
 */
rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn();

/**
 * @fn rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> RequestLevelViewPayload()
 * @brief RTK thunk that seeds data and resolves the full runtime level view
 * payload.
 * User Story: As a features systems consumer, I need to invoke request level view payload through a stable signature so the features systems workflow remains explicit and composable.
 */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

} // namespace RuntimeThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
