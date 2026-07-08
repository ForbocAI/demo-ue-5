#include "Features/Systems/Nature/SystemsNatureActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureActions {

const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &NatureSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FNatureFeatureSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FNatureFeatureSeed>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FNatureFeatureSeed>>(TEXT("nature/seeded"));
      });
  return func::eval(Creator);
}

} // namespace NatureActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
