#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Sky/Stars/StarsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FDistribution,
                       PointStarYawSpanDegrees, PointStarPitchMinDegrees,
                       PointStarPitchSpanDegrees, PointStarDistanceMultiplier,
                       PointStarRollDegrees);
JSON_SETTINGS_REGISTRY(RenderingProfile::FHash, PointStarYawHashSalt,
                       PointStarPitchHashSalt, PointStarSizeHashSalt,
                       PointStarHashIndexOffset, PointStarHashMultiplier);
JSON_SETTINGS_REGISTRY(RenderingProfile::FScale, PointStarCount,
                       PointStarWorldSizeMin,
                       PointStarWorldSizeJitter);
JSON_SETTINGS_REGISTRY(RenderingProfile::FIdentity,
                       RuntimePointStarsActorTag,
                       RuntimePointStarsComponentName);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FPointStars, Distribution,
                               Hash, Scale, Identity);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
