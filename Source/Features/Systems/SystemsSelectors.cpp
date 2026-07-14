#include "Features/Systems/SystemsSelectors.h"

#include "Store.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSelectors {

const FRuntimeState &SelectState() { return Store::GetStore().getState(); }

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
