#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureActions {

/** User Story: As a features systems nature consumer, I need to invoke nature seeded through a stable signature so the features systems nature workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FFeatureSeed>> &NatureSeeded() */
const rtk::ActionCreator<TArray<FFeatureSeed>> &NatureSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FFeatureSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FFeatureSeed>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FFeatureSeed>>(TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV0F4DC9CF9950));
      });
  return func::eval(Creator);
}

} // namespace NatureActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
