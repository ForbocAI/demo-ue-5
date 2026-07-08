#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeProfileApplyRequest {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
  ForbocAI::Game::Data::FRenderingSettings Settings;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
