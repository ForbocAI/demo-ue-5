#pragma once

#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Budget/BudgetFieldAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Labels/LabelFieldAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Measurement/MeasurementFieldAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Presentation/PresentationFieldAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Refresh/RefreshFieldAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Threshold/ThresholdFieldAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_CONCERN_REGISTRY(FOverlaySettings, Labels, Presentation, Refresh,
                               BudgetCapture, Measurement, Thresholds);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
