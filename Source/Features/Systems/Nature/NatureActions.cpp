#include "Features/Systems/Nature/NatureActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureActions {

const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &NatureSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FNatureFeatureSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FNatureFeatureSeed>> {
        return rtk::createAction<TArray<FNatureFeatureSeed>>(TEXT("nature/seeded"));
      });
  return func::eval(Creator);
}

} // namespace NatureActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
