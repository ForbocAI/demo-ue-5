#include "Features/Systems/ForbocAI/Protocol/ProtocolActions.h"

#include "Features/Systems/ForbocAI/Protocol/ProtocolThunks.h"
#include "Features/Systems/SystemsActions.h"
#include "Misc/Guid.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolActions {
namespace {

FString NewCorrelationId() {
  return FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
}

} // namespace

func::AsyncResult<FForbocAINpcResult> DispatchNpcInteraction(
    const FRuntimeTownspersonInteractionSource &Source) {
  return RuntimeDispatch::Dispatch(
      ForbocAIProtocolThunks::RequestNpcInteraction(
          {NewCorrelationId(), Source}));
}

func::AsyncResult<FForbocAIGhostResult> DispatchGhostAnalysis() {
  return RuntimeDispatch::Dispatch(
      ForbocAIProtocolThunks::RequestGhostAnalysis({NewCorrelationId()}));
}

} // namespace ForbocAIProtocolActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
