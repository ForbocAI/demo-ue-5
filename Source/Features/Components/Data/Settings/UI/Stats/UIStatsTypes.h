#pragma once

#include "Features/Components/Data/Settings/UI/Stats/Budget/Capture/BudgetCaptureTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Labels/LabelTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Measurement/MeasurementTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Presentation/PresentationTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Refresh/RefreshTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Threshold/ThresholdTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FOverlaySettings {
  FLabelSettings Labels;
  FStatsPresentationSettings Presentation;
  FRefreshSettings Refresh;
  FCaptureSettings BudgetCapture;
  FMeasurementSettings Measurement;
  FThresholdSettings Thresholds;
};

/** User Story: As a settings UI stats consumer, I need one composed overlay root so labels, presentation, cadence, capture, measurement, and thresholds have explicit ownership. @fn inline bool operator==(const FOverlaySettings &Left, const FOverlaySettings &Right) */
inline bool operator==(const FOverlaySettings &Left,
                       const FOverlaySettings &Right) {
  return Left.Labels == Right.Labels &&
         Left.Presentation == Right.Presentation &&
         Left.Refresh == Right.Refresh &&
         Left.BudgetCapture == Right.BudgetCapture &&
         Left.Measurement == Right.Measurement &&
         Left.Thresholds == Right.Thresholds;
}

/** User Story: As a settings UI stats consumer, I need to compare the composed overlay root for inequality so immutable transitions remain explicit. @fn inline bool operator!=(const FOverlaySettings &Left, const FOverlaySettings &Right) */
inline bool operator!=(const FOverlaySettings &Left,
                       const FOverlaySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
