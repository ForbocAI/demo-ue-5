#include "Features/Components/Data/DataActions.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataActions {

/** User Story: As a features components data consumer, I need to invoke data loaded through a stable signature so the features components data workflow remains explicit and composable. @fn const rtk::ActionCreator<FDataLoadedPayload> &DataLoaded() */
const rtk::ActionCreator<FDataLoadedPayload> &DataLoaded() {
  static const func::Lazy<rtk::ActionCreator<FDataLoadedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FDataLoadedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FDataLoadedPayload>(TEXT("data/loaded"));
      });
  return func::eval(Creator);
}

} // namespace DataActions
} // namespace Data
} // namespace Game
} // namespace ForbocAI
