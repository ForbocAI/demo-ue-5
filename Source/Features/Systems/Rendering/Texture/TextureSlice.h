#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingTextureReducers {

/** User Story: As a systems rendering texture consumer, I need to invoke normalize texture apply through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn FLevelRetroTextureApply NormalizeTextureApply(const FLevelRetroTextureApply &Input) */
FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input);

/**
 * @fn TArray<FLevelRetroTextureSpec> ReduceTextureCatalog( const TArray<ForbocAI::Game::Data::FSpecSettings> &Settings)
 * @brief Maps JSON-backed texture records into rendering texture specs.
 *
 *
 * User story: As an environment artist, prototype material labels and sizes
 * can be changed in JSON without adding view logic.
 * User Story: As a systems rendering texture consumer, I need to invoke reduce texture catalog through a stable signature so the systems rendering texture workflow remains explicit and composable.
 */
TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Game::Data::FSpecSettings>
        &Settings);

/** User Story: As a systems rendering texture consumer, I need to invoke reduce texture spec through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn FLevelRetroTextureSpec ReduceTextureSpec(const FSpecRequest &Request) */
FLevelRetroTextureSpec
ReduceTextureSpec(const FSpecRequest &Request);

} // namespace RenderingTextureReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
