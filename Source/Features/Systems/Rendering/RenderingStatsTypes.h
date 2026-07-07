#pragma once

#include "Features/Components/Data/DataTypes.h"

// Runtime stats view types: request structs used by the stats overlay view.
// Pure data contracts — no behavior, no Unreal widget references.

class UTextBlock;
class UWidgetTree;

namespace ForbocAI {
namespace Game {
namespace Level {

using FRuntimeStatsOverlaySettings =
    ForbocAI::Game::Data::FRuntimeStatsOverlaySettings;

struct FStatsTextElementRequest {
  UWidgetTree *Tree;
  FString Text;
  FLinearColor Color;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FStatsMetricRowRequest {
  UWidgetTree *Tree;
  FString Label;
  UTextBlock **ValueTextElement;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FStatsValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  int32 MediumThreshold;
  int32 HighThreshold;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FStatsDecimalValueUpdateRequest {
  UTextBlock *TextElement;
  double Value;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FStatsPlainValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  const FRuntimeStatsOverlaySettings *Settings;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
