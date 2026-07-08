#include "Features/Components/Data/Selectors.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataSelectors {

func::Maybe<FString> SelectPath(const FDataState &State) {
  return State.Path;
}

bool SelectLoadSucceeded(const FDataState &State) {
  return State.bLoadSucceeded;
}

int32 SelectLoadedObjectCount(const FDataState &State) {
  return State.LoadedObjectCount;
}

} // namespace DataSelectors
} // namespace Data
} // namespace Game
} // namespace ForbocAI
