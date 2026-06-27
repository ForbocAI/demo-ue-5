#include "Features/Components/Data/DataReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataReducers {

FDataState
ReduceDataLoaded(const FDataState &State,
                 const rtk::PayloadAction<FDataLoadedPayload> &Action) {
  FDataState Next = State;
  Next.LastPath = func::just(Action.PayloadValue.RelativePath);
  Next.bLastLoadSucceeded = Action.PayloadValue.bLoaded;
  Next.LoadedObjectCount =
      Action.PayloadValue.bLoaded ? State.LoadedObjectCount + 1
                                  : State.LoadedObjectCount;
  return Next;
}

} // namespace DataReducers
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
