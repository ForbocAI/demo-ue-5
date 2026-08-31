#pragma once

#include "Core/fp.hpp"
#include "Features/Systems/ForbocAI/Protocol/SystemsForbocAIProtocolTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolActions {

/**
 * User Story: As a playable NPC interaction, I need one root-store thunk dispatch for the CLI workflow.
 * @fn func::AsyncResult<FForbocAINpcResult> DispatchNpcInteraction( const FRuntimeTownspersonInteractionSource &Source)
 */
func::AsyncResult<FForbocAINpcResult> DispatchNpcInteraction(
    const FRuntimeTownspersonInteractionSource &Source);

/** User Story: As a playable Ghost analysis, I need one root-store thunk dispatch for the Ghost-only CLI workflow. @fn func::AsyncResult<FForbocAIGhostResult> DispatchGhostAnalysis() */
func::AsyncResult<FForbocAIGhostResult> DispatchGhostAnalysis();

} // namespace ForbocAIProtocolActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
