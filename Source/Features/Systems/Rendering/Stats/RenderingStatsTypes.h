#pragma once

#include "Features/Components/Data/ComponentsDataTypes.h"

// Runtime stats view types: request structs used by the stats overlay view.
// Pure data contracts — no behavior, no Unreal widget references.

class UTextBlock;
class UWidgetTree;

namespace ForbocAI {
namespace Game {
namespace Level {

using FStatsOverlaySettings =
    ForbocAI::Game::Data::FStatsOverlaySettings;

struct FStatsTextElementRequest {
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UWidgetTree *Tree;
  FString Text;
  FLinearColor Color;
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  const FStatsOverlaySettings *Settings;
};

struct FStatsMetricRowRequest {
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UWidgetTree *Tree;
  FString Label;
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UTextBlock **ValueTextElement;
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  const FStatsOverlaySettings *Settings;
};

struct FStatsValueUpdateRequest {
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UTextBlock *TextElement;
  int64 Value;
  int32 MediumThreshold;
  int32 HighThreshold;
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  const FStatsOverlaySettings *Settings;
};

struct FStatsDecimalValueUpdateRequest {
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UTextBlock *TextElement;
  double Value;
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  const FStatsOverlaySettings *Settings;
};

struct FStatsPlainValueUpdateRequest {
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UTextBlock *TextElement;
  int64 Value;
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  const FStatsOverlaySettings *Settings;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
