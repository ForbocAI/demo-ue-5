#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonReducers {

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonState ReduceTownspeopleSeeded( const FTownspersonState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
/** User Story: As a systems bots townspeople consumer, I need to invoke reduce by interaction intent through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn TArray<FTownspersonSeed> ReduceByInteractionIntent( const FTownspersonIntentProjectionRequest &Request) */
TArray<FTownspersonSeed> ReduceByInteractionIntent(
    const FTownspersonIntentProjectionRequest &Request);

/**
 * @fn FTownspersonViewDefaults ReduceViewDefaults( const ForbocAI::Game::Data::FTownspersonDefaultsSettings &Settings)
 * @brief Maps JSON-backed townsperson defaults into RTK state.
 *
 *
 * User story: As a content author, default NPC name, role, persona, prompt,
 * and player line can be edited in JSON without view constants.
 * User Story: As a systems bots townspeople consumer, I need to invoke reduce view defaults through a stable signature so the systems bots townspeople workflow remains explicit and composable.
 */
FTownspersonViewDefaults ReduceViewDefaults(
    const ForbocAI::Game::Data::FTownspersonDefaultsSettings &Settings);

/**
 * @fn FTownspersonViewDefaults ReduceViewDefaults( const FTownspersonViewDefaultsReduceRequest &Request)
 * @brief Applies view-default overrides against the current reducer defaults.
 *
 *
 * User story: As a runtime system, configured NPC spawn data can override only
 * the prompt/line it owns while missing values fall back to store defaults.
 * User Story: As a systems bots townspeople consumer, I need to invoke reduce view defaults through a stable signature so the systems bots townspeople workflow remains explicit and composable.
 */
FTownspersonViewDefaults ReduceViewDefaults(
    const FTownspersonViewDefaultsReduceRequest &Request);
/** User Story: As a systems bots townspeople consumer, I need to invoke reduce interaction overlap through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonInteractionOverlapViewModel ReduceInteractionOverlap( const FTownspersonInteractionOverlapRequest &Request) */
FTownspersonInteractionOverlapViewModel ReduceInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request);

} // namespace TownspersonReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonSlice {

/** User Story: As a systems bots townspeople consumer, I need to invoke get slice through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::Slice<FTownspersonState> &GetSlice() */
const rtk::Slice<FTownspersonState> &GetSlice();

} // namespace TownspersonSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonFactories {

/** User Story: As a systems bots townspeople consumer, I need to invoke create initial state through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonState CreateInitialState() */
FTownspersonState CreateInitialState();

} // namespace TownspersonFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
