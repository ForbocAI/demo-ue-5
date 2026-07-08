#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/View/ViewTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/View/Spawn/ViewSpawnTypes.h"

class UMaterialInterface;
class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {

namespace RenderingThunks {

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id);

void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request);
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

rtk::ThunkAction<void, FRuntimeState>
ObserveRuntimeStatsTick(UWorld *World, float DeltaSeconds);

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
