#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/ForbocAI/Protocol/SystemsForbocAIProtocolTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolSlice {

/** User Story: As the demo protocol boundary, I need one root-owned initial state composed from authored settings. @fn FProtocolState CreateInitialState(const ForbocAI::Game::Data::FProtocolSettings &Settings) */
FProtocolState CreateInitialState(
    const ForbocAI::Game::Data::FProtocolSettings &Settings);

/** User Story: As the demo root store, I need NPC and Ghost CLI lifecycles reduced by one feature slice. @fn const rtk::Slice<FProtocolState> &GetSlice() */
const rtk::Slice<FProtocolState> &GetSlice();

} // namespace ForbocAIProtocolSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
