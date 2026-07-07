#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/RuntimeSettings/UISettingsTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"

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
