#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonActions {

/** User Story: As a systems bots townspeople consumer, I need to invoke townspeople seeded through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded() */
const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FTownspersonSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FTownspersonSeed>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FTownspersonSeed>>(
          TEXT("townspeople/seeded"));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots townspeople consumer, I need to invoke view defaults requested through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::ActionCreator<FTownspersonViewDefaultsRequest> & ViewDefaultsRequested() */
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

/** User Story: As a systems bots townspeople consumer, I need to invoke interaction overlap observed through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::ActionCreator<FTownspersonInteractionOverlapRequest> & InteractionOverlapObserved() */
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
