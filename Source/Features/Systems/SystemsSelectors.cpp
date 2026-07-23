#include "Features/Systems/SystemsSelectors.h"

#include "Store.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSelectors {

/** User Story: As a features systems consumer, I need to invoke select state through a stable signature so the features systems workflow remains explicit and composable. @fn const FRuntimeState &SelectState() */
const FRuntimeState &SelectState() { return Store::GetStore().getState(); }

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
