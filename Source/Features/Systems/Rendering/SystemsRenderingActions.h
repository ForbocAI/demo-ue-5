#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Rendering/Payload/Application/PayloadApplicationAdapters.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

class UMaterialInterface;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingActions {

const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved();
const rtk::ActionCreator<FRuntimeStatsSamplePayload> &RuntimeStatsSampled();
void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request);
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
