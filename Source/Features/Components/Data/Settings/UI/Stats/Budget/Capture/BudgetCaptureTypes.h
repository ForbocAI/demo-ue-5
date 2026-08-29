#pragma once

#include "Features/Components/Data/Settings/UI/Stats/Budget/Capture/Output/OutputTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Budget/Capture/Path/PathTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Budget/Capture/Request/CaptureRequestTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCaptureSettings {
  FCaptureRequestSettings Request;
  FPathSettings Path;
  FOutputSettings Output;
};

/** User Story: As a stats overlay consumer, I need one composed budget capture concern so requests, paths, and output policy share an immutable boundary. @fn inline bool operator==(const FCaptureSettings &Left, const FCaptureSettings &Right) */
inline bool operator==(const FCaptureSettings &Left,
                       const FCaptureSettings &Right) {
  return Left.Request == Right.Request && Left.Path == Right.Path &&
         Left.Output == Right.Output;
}

/** User Story: As a stats overlay consumer, I need to compare composed budget capture settings for inequality so immutable transitions remain explicit. @fn inline bool operator!=(const FCaptureSettings &Left, const FCaptureSettings &Right) */
inline bool operator!=(const FCaptureSettings &Left,
                       const FCaptureSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
