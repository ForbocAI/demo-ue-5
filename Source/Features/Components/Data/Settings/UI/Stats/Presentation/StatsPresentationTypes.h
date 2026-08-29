#pragma once

#include "Features/Components/Data/Settings/UI/Stats/Presentation/Colors/PresentationColorsTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Presentation/Format/PresentationFormatTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Presentation/Message/MessageTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Presentation/Typography/PresentationTypographyTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Presentation/Viewport/PresentationViewportTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FStatsPresentationSettings {
  FPresentationFormatSettings Format;
  FPresentationMessageSettings Message;
  FPresentationViewportSettings Viewport;
  FPresentationTypographySettings Typography;
  FPresentationColorSettings Colors;
};

/** User Story: As a stats overlay consumer, I need one composed presentation concern so formatting and layout share an immutable boundary. @fn inline bool operator==(const FStatsPresentationSettings &Left, const FStatsPresentationSettings &Right) */
inline bool operator==(const FStatsPresentationSettings &Left,
                       const FStatsPresentationSettings &Right) {
  return Left.Format == Right.Format && Left.Message == Right.Message &&
         Left.Viewport == Right.Viewport &&
         Left.Typography == Right.Typography && Left.Colors == Right.Colors;
}

/** User Story: As a stats overlay consumer, I need to compare composed presentation settings for inequality so immutable transitions remain explicit. @fn inline bool operator!=(const FStatsPresentationSettings &Left, const FStatsPresentationSettings &Right) */
inline bool operator!=(const FStatsPresentationSettings &Left,
                       const FStatsPresentationSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
