#pragma once

#include "Features/Components/Rendering/Profile/Sky/Atmosphere/AtmosphereTypes.h"
#include "Features/Components/Rendering/Profile/Sky/Dome/DomeTypes.h"
#include "Features/Components/Rendering/Profile/Sky/Moon/MoonTypes.h"
#include "Features/Components/Rendering/Profile/Sky/Stars/StarsTypes.h"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FSky {
  FAtmosphere Atmosphere;
  FSkyLight Light;
  FSkyDome Dome;
  FMoon Moon;
  FPointStars PointStars;
};

/** User Story: As the rendering profile state owner, I need the composed sky compared structurally so unchanged atmosphere, dome, moon, and star data preserve one projection. @fn inline bool operator==(const FSky &Left, const FSky &Right) */
inline bool operator==(const FSky &Left, const FSky &Right) {
  return Left.Atmosphere == Right.Atmosphere && Left.Light == Right.Light &&
         Left.Dome == Right.Dome && Left.Moon == Right.Moon &&
         Left.PointStars == Right.PointStars;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
