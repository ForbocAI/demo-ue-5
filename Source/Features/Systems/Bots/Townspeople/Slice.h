#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonReducers {

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
TArray<FTownspersonSeed> ReduceByInteractionIntent(
    const FTownspersonIntentProjectionRequest &Request);

/**
 * @brief Maps JSON-backed townsperson defaults into RTK state.
 *
 * @signature FTownspersonViewDefaults ReduceViewDefaults(const
 * ForbocAI::Game::Data::FTownspersonDefaultsSettings &Settings)
 *
 * User story: As a content author, default NPC name, role, persona, prompt,
 * and player line can be edited in JSON without view constants.
 */
FTownspersonViewDefaults ReduceViewDefaults(
    const ForbocAI::Game::Data::FTownspersonDefaultsSettings &Settings);

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
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/Actions.h"
#include "Features/Systems/Bots/Townspeople/Selectors.h"
#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonSlice {

const rtk::Slice<FTownspersonState> &GetSlice();

} // namespace TownspersonSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonFactories {

FTownspersonState CreateInitialState();

} // namespace TownspersonFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
