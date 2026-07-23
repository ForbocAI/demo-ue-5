#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingPresentationReducers {

/**
 * @fn FTownspersonPresentationViewModel ReduceTownspersonPresentation( const FTownspersonPresentationReduceRequest &Request)
 * @brief Maps JSON-backed townsperson presentation settings into view model
 * state.
 *
 * ReduceTownspersonPresentation(const
 * FTownspersonPresentationReduceRequest &Request)
 *
 * User story: As a content author, NPC sizing, text positions, and asset paths
 * can be tuned in JSON while views only apply selected state.
 * User Story: As a systems rendering presentation consumer, I need to invoke reduce townsperson presentation through a stable signature so the systems rendering presentation workflow remains explicit and composable.
 */
FTownspersonPresentationViewModel ReduceTownspersonPresentation(
    const FTownspersonPresentationReduceRequest &Request);

/**
 * @fn FHorsePresentationViewModel ReduceHorsePresentation( const FHorsePresentationReduceRequest &Request)
 * @brief Maps JSON-backed horse presentation settings into view model state.
 *
 *
 * User story: As a content author, horse display assets and movement
 * presentation can be changed in JSON without view hard values.
 * User Story: As a systems rendering presentation consumer, I need to invoke reduce horse presentation through a stable signature so the systems rendering presentation workflow remains explicit and composable.
 */
FHorsePresentationViewModel ReduceHorsePresentation(
    const FHorsePresentationReduceRequest &Request);

} // namespace RenderingPresentationReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
