#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
#include "Features/Systems/Rendering/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingTextureReducers {

FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input);

/**
 * @brief Maps JSON-backed texture records into rendering texture specs.
 *
 * @signature TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(const
 * TArray<ForbocAI::Game::Data::FRenderingTextureSpecSettings> &Settings)
 *
 * User story: As an environment artist, prototype material labels and sizes
 * can be changed in JSON without adding view logic.
 */
TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Game::Data::FRenderingTextureSpecSettings>
        &Settings);

ELevelRetroTexture ReduceTextureKind(const FString &Texture);

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request);

} // namespace RenderingTextureReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
