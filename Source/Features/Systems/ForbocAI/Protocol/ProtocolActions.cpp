#include "Features/Systems/ForbocAI/Protocol/ProtocolActions.h"

#include "Features/Systems/ForbocAI/Protocol/ProtocolThunks.h"
#include "Features/Systems/SystemsActions.h"
#include "Misc/Guid.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolActions {
namespace {

/** User Story: As a systems forboc ai protocol consumer, I need to invoke new correlation id through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FString NewCorrelationId() */
FString NewCorrelationId() {
  return FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
}

} // namespace

/** User Story: As a systems forboc ai protocol consumer, I need to invoke dispatch npc interaction through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn func::AsyncResult<FForbocAINpcResult> DispatchNpcInteraction( const FRuntimeTownspersonInteractionSource &Source) */
func::AsyncResult<FForbocAINpcResult> DispatchNpcInteraction(
    const FRuntimeTownspersonInteractionSource &Source) {
  return RuntimeDispatch::Dispatch(
      ForbocAIProtocolThunks::RequestNpcInteraction(
          {NewCorrelationId(), Source}));
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke dispatch ghost analysis through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn func::AsyncResult<FForbocAIGhostResult> DispatchGhostAnalysis() */
func::AsyncResult<FForbocAIGhostResult> DispatchGhostAnalysis() {
  return RuntimeDispatch::Dispatch(
      ForbocAIProtocolThunks::RequestGhostAnalysis({NewCorrelationId()}));
}

} // namespace ForbocAIProtocolActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
