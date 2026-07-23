#pragma once

#include "CoreMinimal.h"

#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileColorTypes {

using FProfileColorChannel = float FLevelRetroRenderProfile::*;

struct FChannels {
  FProfileColorChannel R;
  FProfileColorChannel G;
  FProfileColorChannel B;
  FProfileColorChannel A;
};

/** User Story: As a rendering profile color consumer, I need to invoke profile linear color through a stable signature so the rendering profile color workflow remains explicit and composable. @fn inline FLinearColor ProfileLinearColor( const FLevelRetroRenderProfile &Profile, const FChannels &Channels) */
inline FLinearColor ProfileLinearColor(
    const FLevelRetroRenderProfile &Profile,
    const FChannels &Channels) {
  return FLinearColor(Profile.*Channels.R, Profile.*Channels.G,
                      Profile.*Channels.B, Profile.*Channels.A);
}

} // namespace RenderingProfileColorTypes
} // namespace Level
} // namespace Game
} // namespace ForbocAI
