#include "Features/Systems/Nature/Entity/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

const rtk::EntityAdapter<FNatureFeatureSeed> &NatureAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FNatureFeatureSeed>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FNatureFeatureSeed> {
        return rtk::createEntityAdapter<FNatureFeatureSeed>(
          [](const FNatureFeatureSeed &Feature) -> FString {
            return Feature.Id;
          });
      });
  return func::eval(Adapter);
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
