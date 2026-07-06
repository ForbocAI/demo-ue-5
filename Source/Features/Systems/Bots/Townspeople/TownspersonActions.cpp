#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonActions {

const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FTownspersonSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FTownspersonSeed>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FTownspersonSeed>>(
          TEXT("townspeople/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FTownspersonViewDefaultsRequest> &
ViewDefaultsRequested() {
  static const func::Lazy<rtk::ActionCreator<FTownspersonViewDefaultsRequest>>
      Creator =
          func::lazy(
              []() -> rtk::ActionCreator<FTownspersonViewDefaultsRequest> {
                // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
                return rtk::createAction<FTownspersonViewDefaultsRequest>(
                    TEXT("townspeople/viewDefaultsRequested"));
              });
  return func::eval(Creator);
}

const rtk::ActionCreator<FTownspersonInteractionOverlapRequest> &
InteractionOverlapObserved() {
  static const func::Lazy<
      rtk::ActionCreator<FTownspersonInteractionOverlapRequest>>
      Creator =
          func::lazy([]()
                         -> rtk::ActionCreator<
                             FTownspersonInteractionOverlapRequest> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FTownspersonInteractionOverlapRequest>(
                TEXT("townspeople/interactionOverlapObserved"));
          });
  return func::eval(Creator);
}

} // namespace TownspersonActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
