#include "Features/Systems/Nature/Entity/EntityAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

/** User Story: As a systems nature entity consumer, I need to invoke nature adapter through a stable signature so the systems nature entity workflow remains explicit and composable. @fn const rtk::EntityAdapter<FFeatureSeed> &NatureAdapter() */
const rtk::EntityAdapter<FFeatureSeed> &NatureAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FFeatureSeed>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FFeatureSeed> {
        return rtk::createEntityAdapter<FFeatureSeed>(
          [](const FFeatureSeed &Feature) -> FString {
            return Feature.Id;
          });
      });
  return func::eval(Adapter);
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
