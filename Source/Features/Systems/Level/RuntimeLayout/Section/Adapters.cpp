#include "Features/Systems/Level/RuntimeLayout/Section/Adapters.h"

#include "Features/Components/Data/Json/Value/Adapters.h"
#include "Features/Systems/Level/RuntimeLayout/Block/Adapters.h"
#include "Features/Systems/Level/RuntimeLayout/Label/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Game::Level::FLevelRuntimeSectionSeed,
                             Blocks, Labels);

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

namespace JsonValues = ForbocAI::Game::Data::JsonValueAdapters;

func::Maybe<FLevelRuntimeSectionSeed>
SectionFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeSectionSeed>({FLevelRuntimeSectionSeed(), Request.Object}, JSON_REQUIRED_ATOMS(Blocks, Labels));
}

func::Maybe<FLevelRuntimeSectionSeed>
ReadSection(const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredValue<TSharedPtr<FJsonObject>>(Request),
      [](const TSharedPtr<FJsonObject> &SectionObject) {
        return SectionFromJson({SectionObject});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
