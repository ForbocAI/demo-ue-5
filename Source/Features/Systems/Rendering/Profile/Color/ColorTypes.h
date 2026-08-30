#pragma once

#include "CoreMinimal.h"

#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileColorTypes {

template <typename Color> struct TChannels {
  float Color::*R;
  float Color::*G;
  float Color::*B;
  float Color::*A;
};

/**
 * User Story: As a rendering profile color consumer, I need one concern-local channel composer so color concerns remain reusable without root-profile member plumbing.
 * @fn template <typename Color> inline FLinearColor ProfileLinearColor( const Color &Value, const TChannels<Color> &Channels)
 */
template <typename Color>
inline FLinearColor ProfileLinearColor(
    const Color &Value, const TChannels<Color> &Channels) {
  return FLinearColor(Value.*Channels.R, Value.*Channels.G,
                      Value.*Channels.B, Value.*Channels.A);
}

} // namespace RenderingProfileColorTypes
} // namespace Level
} // namespace Game
} // namespace ForbocAI
