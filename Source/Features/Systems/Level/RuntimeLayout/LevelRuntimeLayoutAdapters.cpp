#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeSectionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonValueAdapters {

template <>
func::Maybe<ForbocAI::Demo::Level::FLevelRuntimeSectionSeed>
ReadRequiredValue<ForbocAI::Demo::Level::FLevelRuntimeSectionSeed>(
    const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return ForbocAI::Demo::Level::RuntimeLayout::ReadSection(Request);
}

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Demo::Level::FLevelRuntimeLayoutSeed,
                             Terrain, Town, Mine, OverlayLabels);

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

func::Maybe<FLevelRuntimeLayoutSeed>
LayoutFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeLayoutSeed>(
      Request.Object, JSON_REQUIRED_ATOMS(Terrain, Town, Mine, OverlayLabels));
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
