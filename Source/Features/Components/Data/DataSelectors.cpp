#include "Features/Components/Data/DataSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataSelectors {

/** User Story: As a features components data consumer, I need to invoke select path through a stable signature so the features components data workflow remains explicit and composable. @fn func::Maybe<FString> SelectPath(const FDataState &State) */
func::Maybe<FString> SelectPath(const FDataState &State) {
  return State.Path;
}

/** User Story: As a features components data consumer, I need to invoke select load succeeded through a stable signature so the features components data workflow remains explicit and composable. @fn bool SelectLoadSucceeded(const FDataState &State) */
bool SelectLoadSucceeded(const FDataState &State) {
  return State.bLoadSucceeded;
}

/** User Story: As a features components data consumer, I need to invoke select loaded object count through a stable signature so the features components data workflow remains explicit and composable. @fn int32 SelectLoadedObjectCount(const FDataState &State) */
int32 SelectLoadedObjectCount(const FDataState &State) {
  return State.LoadedObjectCount;
}

} // namespace DataSelectors
} // namespace Data
} // namespace Game
} // namespace ForbocAI
