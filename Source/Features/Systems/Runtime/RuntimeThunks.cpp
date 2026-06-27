#include "Features/Systems/Runtime/RuntimeThunks.h"

#include "Features/Components/Level/LevelTypes.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Entities/Characters/Bots/BotsAdapters.h"
#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"
#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Dialogue/DialogueThunks.h"
#include "Features/Systems/Dialogue/DialogueSelectors.h"
#include "Features/Systems/Landmarks/LandmarkActions.h"
#include "Features/Systems/Level/LevelRuntimeSession.h"
#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/UI/UIActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeThunks {
namespace {

void LoadRuntimeData(FLevelTerrainData &TerrainData,
                     FLevelOrthoData &OrthoData) {
  TerrainData.LoadFromContent();
  OrthoData.LoadFromContent();
}

void DispatchRuntimeSeeded(
    const std::function<rtk::AnyAction(const rtk::AnyAction &)> &Dispatch,
    const FLevelTerrainData &TerrainData, const FLevelOrthoData &OrthoData) {
  Dispatch(TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));
  Dispatch(LandmarkActions::LandmarksSeeded()(
      LandmarksAdapters::Build1899LandmarkSeed(TerrainData)));
  Dispatch(SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutSlice::ToWorld(TerrainData,
                                     LevelLayoutSlice::PlayerSpawnPoint()),
           LevelLayoutSlice::PlayerSpawnRotation(),
           LevelLayoutSlice::PlayerSpawnAnchorLabel()})));
  Dispatch(TownspersonActions::TownspeopleSeeded()(
      BotsAdapters::Build1899TownspersonSeed()));
  Dispatch(
      HorseActions::HorsesSeeded()(BotsAdapters::Build1899HorseRouteSeed()));
  Dispatch(NatureActions::NatureSeeded()(
      NatureAdapters::BuildClearCreekNatureSeed()));
}

FDialogueReplyPayload SelectLocalDialogueReplyPayload(
    const FLocalDialogueReplyRequest &Request) {
  FDialogueReplyPayload Payload;
  Payload.Id = FString::Printf(TEXT("systems/dialogue/localReply/%s"),
                               *Request.Name);
  Payload.Request = Request;
  Payload.Reply = DialogueSelectors::SelectLocalReply(Request);
  return Payload;
}

} // namespace

rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return [](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
            std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FSpawnPointPayload> {
    return func::createAsyncResult<FSpawnPointPayload>(
        [Dispatch, GetState](std::function<void(FSpawnPointPayload)> Resolve,
                             std::function<void(std::string)> Reject) {
          (void)Reject;
          FLevelTerrainData TerrainData;
          FLevelOrthoData OrthoData;
          LoadRuntimeData(TerrainData, OrthoData);
          DispatchRuntimeSeeded(Dispatch, TerrainData, OrthoData);
          Resolve(RuntimeSelectors::SelectPlayerSpawn(GetState()));
        });
  };
}

rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload() {
  return [](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
            std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FRuntimeLevelViewPayload> {
    return func::createAsyncResult<FRuntimeLevelViewPayload>(
        [Dispatch, GetState](
            std::function<void(FRuntimeLevelViewPayload)> Resolve,
            std::function<void(std::string)> Reject) {
          (void)Reject;
          FLevelTerrainData TerrainData;
          FLevelOrthoData OrthoData;
          LoadRuntimeData(TerrainData, OrthoData);
          const FLevelRuntimeLayoutSeed RuntimeLayout =
              LevelRuntimeSession::BuildFrenchGulchRuntimeLayoutSeed();
          DispatchRuntimeSeeded(Dispatch, TerrainData, OrthoData);
          func::executeAsync(RenderingThunks::ObserveRuntimeProfile(
                                 TEXT("runtime/rendering/profileObserved"))(
              Dispatch, GetState));
          Resolve(RuntimeSelectors::SelectLevelViewPayload(
              GetState(), {&TerrainData, &OrthoData, &RuntimeLayout}));
        });
  };
}

rtk::ThunkAction<FDialogueReplyPayload, FRuntimeState>
RequestLocalDialogueReply(const FLocalDialogueReplyRequest &Request) {
  return [Request](std::function<rtk::AnyAction(const rtk::AnyAction &)>
                       Dispatch,
                   std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FDialogueReplyPayload> {
    (void)GetState;
    return func::createAsyncResult<FDialogueReplyPayload>(
        [Dispatch, Request](
            std::function<void(FDialogueReplyPayload)> Resolve,
            std::function<void(std::string)> Reject) {
          (void)Reject;
          Dispatch(DialogueThunks::RequestLocalReply().pending(Request));
          const FDialogueReplyPayload Payload =
              SelectLocalDialogueReplyPayload(Request);
          Dispatch(DialogueThunks::RequestLocalReply().fulfilled(Payload));
          Resolve(Payload);
        });
  };
}

rtk::ThunkAction<FRuntimeTownspersonInteractionPayload, FRuntimeState>
RequestTownspersonInteraction(
    const FRuntimeTownspersonInteractionRequest &Request) {
  return [Request](std::function<rtk::AnyAction(const rtk::AnyAction &)>
                       Dispatch,
                   std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FRuntimeTownspersonInteractionPayload> {
    (void)GetState;
    return func::createAsyncResult<FRuntimeTownspersonInteractionPayload>(
        [Dispatch, Request](
            std::function<void(FRuntimeTownspersonInteractionPayload)> Resolve,
            std::function<void(std::string)> Reject) {
          (void)Reject;
          const FLocalDialogueReplyRequest DialogueRequest =
              RuntimeSelectors::SelectLocalDialogueReplyRequest(Request);
          Dispatch(
              DialogueThunks::RequestLocalReply().pending(DialogueRequest));
          const FDialogueReplyPayload DialogueReply =
              SelectLocalDialogueReplyPayload(DialogueRequest);
          Dispatch(DialogueThunks::RequestLocalReply().fulfilled(
              DialogueReply));
          const FRuntimeTownspersonInteractionPayload Payload =
              RuntimeSelectors::SelectTownspersonInteractionPayload(
                  DialogueReply);
          Dispatch(UIActions::ConversationPresented()(Payload.UI));
          Resolve(Payload);
        });
  };
}

} // namespace RuntimeThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
