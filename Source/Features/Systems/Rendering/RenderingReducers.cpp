#include "Features/Systems/Rendering/RenderingReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingReducers {

FLevelRetroTextureApply NormalizeTextureApply(const FLevelRetroTextureApply &Input) {
  return (func::pipe(Input) | [](FLevelRetroTextureApply Value) -> FLevelRetroTextureApply {
    return Value;
  }).val;
}

} // namespace RenderingReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
