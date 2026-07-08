#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingPresentationReducers {

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

} // namespace RenderingPresentationReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
