#include "Features/Components/Data/DataActions.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataActions {

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
