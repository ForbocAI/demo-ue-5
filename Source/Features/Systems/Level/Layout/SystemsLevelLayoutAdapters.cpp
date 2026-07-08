#include "Features/Systems/Level/Layout/SystemsLevelLayoutAdapters.h"

#include "Features/Components/Data/Json/Value/ValueAdapters.h"
#include "Features/Systems/Level/Layout/Block/BlockAdapters.h"
#include "Features/Systems/Level/Layout/Label/LabelAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {
namespace {

namespace JsonValues = ForbocAI::Game::Data::JsonValueAdapters;

template <typename Output>
TArray<Output> ReadLeafObjects(
    const TArray<TSharedPtr<FJsonValue>> &Values,
    const FString &LeafName,
    TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> Read) {
  const func::Maybe<TArray<Output>> Result =
      JsonValues::MapRequiredJsonValuesWith<Output>(LeafName, Read)(Values);
  check(Result.hasValue);
  return Result.value;
}

FSectionSeed
SectionFromLeaves(const FLevelSectionLeaves &Leaves) {
  FSectionSeed Seed;
  Seed.Blocks = ReadLeafObjects<FBlockSeed>(
      Leaves.Blocks, TEXT("blocks"),
      [](const TSharedPtr<FJsonObject> &Object) {
        return BlockFromJson({Object});
      });
  Seed.Labels = ReadLeafObjects<FLabelSeed>(
      Leaves.Labels, TEXT("labels"),
      [](const TSharedPtr<FJsonObject> &Object) {
        return LabelFromJson({Object});
      });
  return Seed;
}

} // namespace

func::Maybe<FLayoutSeed>
LayoutFromJson(const FLeaves &Leaves) {
  FLayoutSeed Seed;
  Seed.Terrain = SectionFromLeaves(Leaves.Terrain);
  Seed.Town = SectionFromLeaves(Leaves.Town);
  Seed.Mine = SectionFromLeaves(Leaves.Mine);
  Seed.OverlayLabels = ReadLeafObjects<FLabelSeed>(
      Leaves.OverlayLabels, TEXT("overlay_labels"),
      [](const TSharedPtr<FJsonObject> &Object) {
        return LabelFromJson({Object});
      });
  return func::just(Seed);
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
