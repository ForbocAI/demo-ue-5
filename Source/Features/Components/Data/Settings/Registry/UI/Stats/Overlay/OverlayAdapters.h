#pragma once

#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Budget/BudgetAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Labels/LabelsAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Measurement/MeasurementAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Presentation/PresentationAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Refresh/RefreshAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/Overlay/Threshold/ThresholdAdapters.h"

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
