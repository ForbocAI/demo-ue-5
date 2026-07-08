#pragma once

#include "CoreMinimal.h"

#include "Features/Components/Rendering/Profile/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileColorTypes {

using FProfileColorChannel = float FLevelRetroRenderProfile::*;

struct FProfileColorChannels {
  FProfileColorChannel R;
  FProfileColorChannel G;
  FProfileColorChannel B;
  FProfileColorChannel A;
};

inline FLinearColor ProfileLinearColor(
    const FLevelRetroRenderProfile &Profile,
    const FProfileColorChannels &Channels) {
  return FLinearColor(Profile.*Channels.R, Profile.*Channels.G,
                      Profile.*Channels.B, Profile.*Channels.A);
}

} // namespace RenderingProfileColorTypes
} // namespace Level
} // namespace Game
} // namespace ForbocAI
