#include "Features/Systems/Runtime/RuntimeReducers.h"

#include "Features/Systems/SystemsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceRuntimeEcsProjected(const FRuntimeState &State) {
  return (func::pipe(State) |
          [](FRuntimeState Next) -> FRuntimeState {
            Next.Ecs.World = SystemsAdapters::ProjectRuntimeEcsWorld(Next);
            return Next;
          })
      .val;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
