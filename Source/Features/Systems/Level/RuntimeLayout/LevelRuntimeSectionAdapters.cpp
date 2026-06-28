#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeSectionAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeBlockAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

struct FLevelRuntimeSectionArrayRequest {
  TArray<TSharedPtr<FJsonValue>> Blocks;
  TArray<TSharedPtr<FJsonValue>> Labels;
};

/**
 * @brief Maps section block and label arrays into typed seed arrays.
 *
 * @signature func::Maybe<FLevelRuntimeSectionSeed> BuildSectionFromArrays(const FLevelRuntimeSectionArrayRequest &Request)
 *
 * User story: As a Level adapter maintainer, repeated authored objects are
 * mapped through neutral Data primitives before reducers receive section data.
 */
func::Maybe<FLevelRuntimeSectionSeed>
BuildSectionFromArrays(const FLevelRuntimeSectionArrayRequest &Request) {
  const func::Maybe<TArray<FLevelRuntimeBlockSeed>> ParsedBlocks =
      JsonValues::MapRequiredJsonValues<FLevelRuntimeBlockSeed>(
          {Request.Blocks,
           [](const TSharedPtr<FJsonObject> &BlockObject) {
             return BlockFromJson({BlockObject});
           },
           TEXT("blocks")});
  const func::Maybe<TArray<FLevelRuntimeLabelSeed>> ParsedLabels =
      JsonValues::MapRequiredJsonValues<FLevelRuntimeLabelSeed>(
          {Request.Labels,
           [](const TSharedPtr<FJsonObject> &LabelObject) {
             return LabelFromJson({LabelObject});
           },
           TEXT("labels")});
  return ParsedBlocks.hasValue && ParsedLabels.hasValue
             ? func::just(FLevelRuntimeSectionSeed{ParsedBlocks.value,
                                                   ParsedLabels.value})
             : func::nothing<FLevelRuntimeSectionSeed>();
}

} // namespace

func::Maybe<FLevelRuntimeSectionSeed>
SectionFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Blocks =
      JsonValues::ReadRequiredArray({Request.Object, TEXT("blocks")});
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Labels =
      JsonValues::ReadRequiredArray({Request.Object, TEXT("labels")});
  return Blocks.hasValue && Labels.hasValue
             ? BuildSectionFromArrays({Blocks.value, Labels.value})
             : func::nothing<FLevelRuntimeSectionSeed>();
}

func::Maybe<FLevelRuntimeSectionSeed>
ReadSection(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredObject(Request),
      [](const TSharedPtr<FJsonObject> &SectionObject) {
        return SectionFromJson({SectionObject});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
