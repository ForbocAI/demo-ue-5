#include "Features/Components/Data/DataSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataSelectors {

func::Maybe<FString> SelectLastPath(const FDataState &State) {
  return State.LastPath;
}

bool SelectLastLoadSucceeded(const FDataState &State) {
  return State.bLastLoadSucceeded;
}

int32 SelectLoadedObjectCount(const FDataState &State) {
  return State.LoadedObjectCount;
}

} // namespace DataSelectors
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
