#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeSectionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

func::Maybe<FLevelRuntimeLabelSeed>
ReadOverlayLabelObject(const TSharedPtr<FJsonObject> &Object) {
  return LabelFromJson({Object});
}

} // namespace

func::Maybe<FLevelRuntimeLayoutSeed>
LayoutFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeLayoutSeed>(
      Request.Object,
      {JSON_REQUIRED_FIELD_READERS(FLevelRuntimeLayoutSeed, ReadSection,
                                   Terrain, Town, Mine),
       JSON_REQUIRED_FIELD_READER(FLevelRuntimeLayoutSeed,
                                  ReadOverlayLabelObject, OverlayLabels)});
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
