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

/** User Story: As a features systems rendering consumer, I need to invoke rendering profile observed through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved() */
const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved();
/** User Story: As a features systems rendering consumer, I need to invoke runtime stats sampled through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const rtk::ActionCreator<FRuntimeStatsSamplePayload> &RuntimeStatsSampled() */
const rtk::ActionCreator<FRuntimeStatsSamplePayload> &RuntimeStatsSampled();
/** User Story: As a features systems rendering consumer, I need to invoke apply runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) */
void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request);
/** User Story: As a features systems rendering consumer, I need to invoke load blockout material through a stable signature so the features systems rendering workflow remains explicit and composable. @fn UMaterialInterface *LoadBlockoutMaterial(const FString &Path) */
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
/** User Story: As a features systems rendering consumer, I need to invoke apply texture through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyTexture(const FLevelRetroTextureApply &Request) */
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
