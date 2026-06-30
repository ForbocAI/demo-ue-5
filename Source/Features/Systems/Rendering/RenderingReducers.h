#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingReducers {

FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input);

/**
 * @brief Maps JSON-backed runtime rendering settings into RTK state.
 *
 * @signature FLevelRetroRenderProfile ReduceRuntimeProfile(const
 * ForbocAI::Demo::Data::FRenderingProfileSettings &Settings)
 *
 * User story: As a demo operator, runtime rendering quality can be tuned from
 * data while the rendering slice remains the owner of store semantics.
 */
FLevelRetroRenderProfile ReduceRuntimeProfile(
    const ForbocAI::Demo::Data::FRenderingProfileSettings &Settings);

/**
 * @brief Maps JSON-backed texture records into rendering texture specs.
 *
 * @signature TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(const
 * TArray<ForbocAI::Demo::Data::FRenderingTextureSpecSettings> &Settings)
 *
 * User story: As an environment artist, prototype material labels and sizes
 * can be changed in JSON without adding view logic.
 */
TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Demo::Data::FRenderingTextureSpecSettings>
        &Settings);

/**
 * @brief Maps JSON-backed asset path settings into rendering state.
 *
 * @signature FRenderingAssetPaths ReduceRenderingAssetPaths(const
 * ForbocAI::Demo::Data::FRenderingAssetPathSettings &Settings)
 *
 * User story: As a level-view maintainer, mesh/material references stay in
 * store-owned data instead of view constructors.
 */
FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Demo::Data::FRenderingAssetPathSettings &Settings);

ELevelRetroTexture ReduceTextureKind(const FString &Texture);

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request);

FRenderingPayload ReduceRenderingPayload(
    const FRenderingPayloadRequest &Request);

FRenderingState
ReduceRenderingProfileObserved(const FRenderingState &State,
                               const rtk::PayloadAction<FRenderingPayload>
                                   &Action);
FRenderingState ReduceTownspersonPresentationRequested(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPresentationRequest> &Action);
FRenderingState ReduceHorsePresentationRequested(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPresentationRequest> &Action);

/**
 * @brief Maps JSON-backed townsperson presentation settings into view model
 * state.
 *
 * @signature FTownspersonPresentationViewModel
 * ReduceTownspersonPresentation(const
 * FTownspersonPresentationReduceRequest &Request)
 *
 * User story: As a content author, NPC sizing, text positions, and asset paths
 * can be tuned in JSON while views only apply selected state.
 */
FTownspersonPresentationViewModel ReduceTownspersonPresentation(
    const FTownspersonPresentationReduceRequest &Request);

/**
 * @brief Maps JSON-backed horse presentation settings into view model state.
 *
 * @signature FHorsePresentationViewModel ReduceHorsePresentation(const
 * FHorsePresentationReduceRequest &Request)
 *
 * User story: As a content author, horse display assets and movement
 * presentation can be changed in JSON without view hard values.
 */
FHorsePresentationViewModel ReduceHorsePresentation(
    const FHorsePresentationReduceRequest &Request);

} // namespace RenderingReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
