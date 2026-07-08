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

struct FTextElementRequest {
  UWidgetTree *Tree;
  FString Text;
  FLinearColor Color;
  const FStatsOverlaySettings *Settings;
};

struct FMetricRowRequest {
  UWidgetTree *Tree;
  FString Label;
  UTextBlock **ValueTextElement;
  const FStatsOverlaySettings *Settings;
};

struct FValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  int32 MediumThreshold;
  int32 HighThreshold;
  const FStatsOverlaySettings *Settings;
};

struct FDecimalValueUpdateRequest {
  UTextBlock *TextElement;
  double Value;
  const FStatsOverlaySettings *Settings;
};

struct FPlainValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  const FStatsOverlaySettings *Settings;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
