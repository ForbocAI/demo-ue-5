#include "Features/Components/Data/DataActions.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataActions {

const rtk::ActionCreator<FDataLoadedPayload> &DataLoaded() {
  static const func::Lazy<rtk::ActionCreator<FDataLoadedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FDataLoadedPayload> {
        return rtk::createAction<FDataLoadedPayload>(TEXT("data/loaded"));
      });
  return func::eval(Creator);
}

} // namespace DataActions
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
