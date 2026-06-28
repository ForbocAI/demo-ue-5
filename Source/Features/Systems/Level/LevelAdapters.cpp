#include "Features/Systems/Level/LevelAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

FLevelRuntimeLayoutSeed LoadRuntimeLayoutSeed(const FString &RelativeJsonPath) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({RelativeJsonPath});
  const func::Maybe<FLevelRuntimeLayoutSeed> Layout =
      RuntimeLayout::LayoutFromJson({Root});
  checkf(Layout.hasValue, TEXT("Runtime layout JSON is invalid: %s"),
         *RelativeJsonPath);
  return Layout.value;
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
