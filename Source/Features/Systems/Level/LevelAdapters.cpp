#include "Features/Systems/Level/LevelAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

namespace {

constexpr const TCHAR *RuntimeLevelDataPath =
    TEXT("Data/french_gulch_runtime_level.json");

} // namespace

func::Maybe<FLevelRuntimeLayoutSeed> LoadFrenchGulchRuntimeLayoutSeed() {
  return func::mbind(
      JsonAdapters::LoadObjectFromContent({RuntimeLevelDataPath}),
      [](const TSharedPtr<FJsonObject> &Root) {
        return RuntimeLayout::LayoutFromJson({Root});
      });
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
