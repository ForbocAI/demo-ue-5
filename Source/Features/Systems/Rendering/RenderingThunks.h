#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Rendering/Payload/Application/PayloadApplicationAdapters.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

class UMaterialInterface;
class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {

namespace RenderingThunks {

/** User Story: As a features systems rendering consumer, I need to invoke observe runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn rtk::ThunkAction<FRenderingPayload, FRuntimeState> ObserveRuntimeProfile(const FString &Id) */
rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id);

/** User Story: As a features systems rendering consumer, I need to invoke apply runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) */
void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request);
/** User Story: As a features systems rendering consumer, I need to invoke load blockout material through a stable signature so the features systems rendering workflow remains explicit and composable. @fn UMaterialInterface *LoadBlockoutMaterial(const FString &Path) */
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
/** User Story: As a features systems rendering consumer, I need to invoke apply texture through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyTexture(const FLevelRetroTextureApply &Request) */
void ApplyTexture(const FLevelRetroTextureApply &Request);

/** User Story: As a features systems rendering consumer, I need to invoke observe runtime stats tick through a stable signature so the features systems rendering workflow remains explicit and composable. @fn rtk::ThunkAction<void, FRuntimeState> ObserveRuntimeStatsTick(UWorld *World, float DeltaSeconds) */
rtk::ThunkAction<void, FRuntimeState>
ObserveRuntimeStatsTick(UWorld *World, float DeltaSeconds);

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
