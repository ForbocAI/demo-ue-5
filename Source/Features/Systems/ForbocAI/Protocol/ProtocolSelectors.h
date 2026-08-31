#pragma once

#include "Core/fp.hpp"
#include "Features/Systems/ForbocAI/Protocol/SystemsForbocAIProtocolTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolSelectors {

/**
 * User Story: As protocol presentation, I need reducer-owned flow evidence projected into one display model.
 * @fn FForbocAIFlowPresentation SelectFlowPresentation( const FForbocAIFlowState &Flow, const ForbocAI::Game::Data::FForbocAIFlowPresentationSettings &Settings)
 */
FForbocAIFlowPresentation SelectFlowPresentation(
    const FForbocAIFlowState &Flow,
    const ForbocAI::Game::Data::FForbocAIFlowPresentationSettings &Settings);

/** User Story: As NPC presentation, I need the current root-owned CLI state projected for the view. @fn FForbocAIFlowPresentation SelectNpcPresentation(const FRuntimeState &State) */
FForbocAIFlowPresentation SelectNpcPresentation(const FRuntimeState &State);
/** User Story: As a systems forboc ai protocol consumer, I need to invoke select npc presentation through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowPresentation SelectNpcPresentation() */
FForbocAIFlowPresentation SelectNpcPresentation();

/** User Story: As Ghost presentation, I need the current root-owned CLI state projected for the view. @fn FForbocAIFlowPresentation SelectGhostPresentation(const FRuntimeState &State) */
FForbocAIFlowPresentation SelectGhostPresentation(const FRuntimeState &State);
/** User Story: As a systems forboc ai protocol consumer, I need to invoke select ghost presentation through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowPresentation SelectGhostPresentation() */
FForbocAIFlowPresentation SelectGhostPresentation();

/** User Story: As redux diagnostics, I need protocol evidence projected into a credential-free state summary. @fn FString SelectProtocolLogSummary(const FRuntimeState &State) */
FString SelectProtocolLogSummary(const FRuntimeState &State);

/** User Story: As bot action execution, I need the SDK CLI action line projected without interpreting API cognition. @fn func::Maybe<FString> SelectNpcAction(const FString &Output, const FString &ActionLinePrefix) */
func::Maybe<FString> SelectNpcAction(const FString &Output,
                                     const FString &ActionLinePrefix);

} // namespace ForbocAIProtocolSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
