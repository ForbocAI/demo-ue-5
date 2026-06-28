#include "Features/Systems/Level/LevelAdapters.h"

#include "Features/Components/Data/DataAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelAdapters {

namespace DataAdapters = ForbocAI::Demo::Data::DataAdapters;

namespace {

constexpr const TCHAR *RuntimeLevelDataPath =
    TEXT("Data/french_gulch_runtime_level.json");

} // namespace

func::Maybe<FLevelRuntimeLayoutSeed> LoadFrenchGulchRuntimeLayoutSeed() {
  return func::mbind(
      DataAdapters::LoadObjectFromContent({RuntimeLevelDataPath}),
      [](const TSharedPtr<FJsonObject> &Root) {
        return RuntimeLayout::LayoutFromJson({Root});
      });
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
