#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/AITypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIReducers {

/** User Story: As a systems bots ai consumer, I need to invoke reduce bot aiseeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceBotAISeeded( const FBotAIState &State, const rtk::PayloadAction<TArray<FBotAIComponent>> &Action) */
FBotAIState ReduceBotAISeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FBotAIComponent>> &Action);
/** User Story: As a systems bots ai consumer, I need to invoke reduce bot aiupdated through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceBotAIUpdated(const FBotAIState &State, const rtk::PayloadAction<FBotAIUpdated> &Action) */
FBotAIState ReduceBotAIUpdated(const FBotAIState &State,
                               const rtk::PayloadAction<FBotAIUpdated> &Action);
/** User Story: As a systems bots ai consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceTownspeopleSeeded( const FBotAIState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FBotAIState ReduceTownspeopleSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
/** User Story: As a systems bots ai consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceHorsesSeeded( const FBotAIState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
FBotAIState ReduceHorsesSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotAIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/AIActions.h"
#include "Features/Systems/Bots/AI/AISelectors.h"
#include "Features/Systems/Bots/AI/AITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAISlice {

/** User Story: As a systems bots ai consumer, I need to invoke get slice through a stable signature so the systems bots ai workflow remains explicit and composable. @fn const rtk::Slice<FBotAIState> &GetSlice() */
const rtk::Slice<FBotAIState> &GetSlice();

} // namespace BotAISlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/AI/AITypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIFactories {

/** User Story: As a systems bots ai consumer, I need to invoke create initial state through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState CreateInitialState() */
FBotAIState CreateInitialState();
/** User Story: As a systems bots ai consumer, I need to invoke component through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIComponent Component(const FBotAISource &Source) */
FBotAIComponent Component(const FBotAISource &Source);
/** User Story: As a systems bots ai consumer, I need to invoke from townspeople through a stable signature so the systems bots ai workflow remains explicit and composable. @fn TArray<FBotAIComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) */
TArray<FBotAIComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
/** User Story: As a systems bots ai consumer, I need to invoke from horses through a stable signature so the systems bots ai workflow remains explicit and composable. @fn TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) */
TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotAIFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
