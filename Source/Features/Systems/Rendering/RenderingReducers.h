#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingReducers {

FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input);
const FLevelRetroRenderProfile &ReduceRuntimeProfileDefaults();
TArray<FLevelRetroTextureSpec> ReduceTextureCatalogDefaults();
FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request);
FLevelRetroTextureSpec
ReduceDefaultTextureSpec(const FRenderingDefaultTextureSpecRequest &Request);

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
FTownspersonPresentationViewModel ReduceTownspersonPresentation(
    const FRenderingPresentationRequest &Request);
FHorsePresentationViewModel ReduceHorsePresentation(
    const FRenderingPresentationRequest &Request);

} // namespace RenderingReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
