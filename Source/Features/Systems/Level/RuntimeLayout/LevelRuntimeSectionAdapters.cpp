#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeSectionAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeBlockAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Demo::Level::FLevelRuntimeSectionSeed,
                             Blocks, Labels);

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

func::Maybe<FLevelRuntimeSectionSeed>
SectionFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeSectionSeed>(
      Request.Object, JSON_REQUIRED_ATOMS(Blocks, Labels));
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
