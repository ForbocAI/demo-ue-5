#include "Features/Systems/Nature/NatureActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureActions {

const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &NatureSeeded() {
  static const rtk::ActionCreator<TArray<FNatureFeatureSeed>> Creator =
      rtk::createAction<TArray<FNatureFeatureSeed>>(TEXT("nature/seeded"));
  return Creator;
}

} // namespace NatureActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
