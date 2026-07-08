#include "Features/Systems/Nature/SystemsNatureActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureActions {

const rtk::ActionCreator<TArray<FFeatureSeed>> &NatureSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FFeatureSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FFeatureSeed>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FFeatureSeed>>(TEXT("nature/seeded"));
      });
  return func::eval(Creator);
}

} // namespace NatureActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
