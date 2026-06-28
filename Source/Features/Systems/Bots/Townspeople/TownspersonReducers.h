#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonReducers {

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FTownspersonState ReduceViewDefaultsRequested(
    const FTownspersonState &State,
    const rtk::PayloadAction<FTownspersonViewDefaultsRequest> &Action);
FTownspersonState ReduceInteractionOverlapObserved(
    const FTownspersonState &State,
    const rtk::PayloadAction<FTownspersonInteractionOverlapRequest> &Action);
TArray<FTownspersonSeed> ReduceByInteractionIntent(
    const FTownspersonIntentProjectionRequest &Request);

/**
 * @brief Maps JSON-backed townsperson defaults into RTK state.
 *
 * @signature FTownspersonViewDefaults ReduceViewDefaults(const
 * ForbocAI::Demo::Data::FTownspersonDefaultsSettings &Settings)
 *
 * User story: As a content author, default NPC name, role, persona, prompt,
 * and player line can be edited in JSON without view constants.
 */
FTownspersonViewDefaults ReduceViewDefaults(
    const ForbocAI::Demo::Data::FTownspersonDefaultsSettings &Settings);

/**
 * @brief Applies view-default overrides against the current reducer defaults.
 *
 * @signature FTownspersonViewDefaults ReduceViewDefaults(const
 * FTownspersonViewDefaultsReduceRequest &Request)
 *
 * User story: As a runtime system, configured NPC spawn data can override only
 * the prompt/line it owns while missing values fall back to store defaults.
 */
FTownspersonViewDefaults ReduceViewDefaults(
    const FTownspersonViewDefaultsReduceRequest &Request);
FTownspersonInteractionOverlapViewModel ReduceInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request);

} // namespace TownspersonReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
