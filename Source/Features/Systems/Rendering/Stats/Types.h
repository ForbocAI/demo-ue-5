#pragma once

#include "Features/Components/Data/Types.h"

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
  UWidgetTree *Tree;
  FString Text;
  FLinearColor Color;
  const FStatsOverlaySettings *Settings;
};

struct FStatsMetricRowRequest {
  UWidgetTree *Tree;
  FString Label;
  UTextBlock **ValueTextElement;
  const FStatsOverlaySettings *Settings;
};

struct FStatsValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  int32 MediumThreshold;
  int32 HighThreshold;
  const FStatsOverlaySettings *Settings;
};

struct FStatsDecimalValueUpdateRequest {
  UTextBlock *TextElement;
  double Value;
  const FStatsOverlaySettings *Settings;
};

struct FStatsPlainValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  const FStatsOverlaySettings *Settings;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
