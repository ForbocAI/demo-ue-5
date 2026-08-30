#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/ForbocAI/Protocol/SystemsForbocAIProtocolTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolThunks {

/** User Story: As NPC protocol state, I need the CLI interaction lifecycle exposed for reducer composition. @fn const rtk::AsyncThunkConfig<FForbocAINpcResult, FForbocAINpcRequest, FRuntimeState> &NpcInteractionAsyncThunk() */
const rtk::AsyncThunkConfig<FForbocAINpcResult, FForbocAINpcRequest,
                            FRuntimeState> &
NpcInteractionAsyncThunk();

/** User Story: As Ghost protocol state, I need the CLI level-analysis lifecycle exposed for reducer composition. @fn const rtk::AsyncThunkConfig<FForbocAIGhostResult, FForbocAIGhostRequest, FRuntimeState> &GhostAnalysisAsyncThunk() */
const rtk::AsyncThunkConfig<FForbocAIGhostResult, FForbocAIGhostRequest,
                            FRuntimeState> &
GhostAnalysisAsyncThunk();

/** User Story: As an NPC interaction caller, I need one request converted into the RTK async thunk owned by the ForbocAI protocol feature. @fn rtk::ThunkAction<FForbocAINpcResult, FRuntimeState> RequestNpcInteraction(const FForbocAINpcRequest &Request) */
rtk::ThunkAction<FForbocAINpcResult, FRuntimeState>
RequestNpcInteraction(const FForbocAINpcRequest &Request);

/** User Story: As a Ghost caller, I need one request converted into the RTK async thunk owned by the ForbocAI protocol feature. @fn rtk::ThunkAction<FForbocAIGhostResult, FRuntimeState> RequestGhostAnalysis(const FForbocAIGhostRequest &Request) */
rtk::ThunkAction<FForbocAIGhostResult, FRuntimeState>
RequestGhostAnalysis(const FForbocAIGhostRequest &Request);

} // namespace ForbocAIProtocolThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
