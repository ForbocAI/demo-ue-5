#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Core/ue_fp.hpp"
#include "Features/Systems/Rendering/SystemsRenderingActions.h"
#include "Features/Systems/Rendering/Profile/Runtime/RuntimeThunks.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/SystemsSelectors.h"

// Runtime render profile orchestrator. The subdomain application passes live
// in sibling translation units and share RenderingProfileRuntimeThunks.h:
// - RenderingProfileConsoleThunks.cpp  console variable settings
// - RenderingProfileWorldThunks.cpp    directional light + exponential fog
// - RenderingProfileSkyThunks.cpp      atmosphere/light/dome/moon/stars
// - RenderingProfileOutputThunks.cpp   post-process volume + render output
DEFINE_LOG_CATEGORY(LogForbocRenderingProfile);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id) {
  return [Id](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
              std::function<const FRuntimeState &()> GetState)
             -> func::AsyncResult<FRenderingPayload> {
    const FRuntimeState &State = GetState();
    const FRenderingPayload Payload =
        RenderingReducers::ReduceRenderingPayload(
            {Id, RuntimeSelectors::SelectRuntimeProfile(State),
             RuntimeSelectors::SelectTextureCatalog(State),
             RuntimeSelectors::SelectDistanceLodStages(State),
             RuntimeSelectors::SelectRenderingSettings(State)});
    return func::createAsyncResult<FRenderingPayload>(
        [Dispatch, Payload](std::function<void(FRenderingPayload)> Resolve,
                            std::function<void(std::string)> Reject) {
          Dispatch(RenderingActions::RenderingProfileObserved()(Payload));
          Resolve(Payload);
        });
  };
}

void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) {
  const FRuntimeProfileEval Eval{Request.World, Request.Profile};
  ApplyRuntimeConsoleVariables(Request.Profile, Request.Settings);
  ApplyRuntimeOutput(Eval);
  ApplyRuntimeLighting(Eval);
  ApplyRuntimeSky(Eval);
  ApplyRuntimePostProcess(Eval);
  ApplyRuntimeFog(Eval);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
