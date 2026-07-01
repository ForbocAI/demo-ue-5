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

func::Maybe<FLevelRuntimeBlockSeed>
ReadBlockObject(const TSharedPtr<FJsonObject> &Object) {
  return BlockFromJson({Object});
}

func::Maybe<FLevelRuntimeLabelSeed>
ReadLabelObject(const TSharedPtr<FJsonObject> &Object) {
  return LabelFromJson({Object});
}

} // namespace

func::Maybe<FLevelRuntimeSectionSeed>
SectionFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeSectionSeed>(
      Request.Object,
      {JSON_REQUIRED_FIELD_READER(FLevelRuntimeSectionSeed, ReadBlockObject,
                                  Blocks),
       JSON_REQUIRED_FIELD_READER(FLevelRuntimeSectionSeed, ReadLabelObject,
                                  Labels)});
}

func::Maybe<FLevelRuntimeSectionSeed>
ReadSection(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredValue<TSharedPtr<FJsonObject>>(Request),
      [](const TSharedPtr<FJsonObject> &SectionObject) {
        return SectionFromJson({SectionObject});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
