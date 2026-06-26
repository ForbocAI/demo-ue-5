#include "Features/Systems/Nature/NatureAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureAdapters {

const rtk::EntityAdapterOps<FNatureFeatureSeed> &NatureAdapter() {
  static const rtk::EntityAdapterOps<FNatureFeatureSeed> Adapter =
      rtk::createEntityAdapter<FNatureFeatureSeed>(
          [](const FNatureFeatureSeed &Feature) -> FString {
            return Feature.Id;
          });
  return Adapter;
}

} // namespace NatureAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
