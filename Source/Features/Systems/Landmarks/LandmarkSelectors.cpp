#include "Features/Systems/Landmarks/LandmarkSelectors.h"

#include "Features/Systems/Landmarks/LandmarkAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSelectors {

TArray<FLandmark> SelectAll(const FLandmarkState &State) {
  return LandmarkAdapters::LandmarkAdapter().getSelectors().selectAll(
      State.Items);
}

func::Maybe<FLandmark> SelectById(const FLandmarkState &State,
                                  const FString &Id) {
  return LandmarkAdapters::LandmarkAdapter().getSelectors().selectById(
      State.Items, Id);
}

} // namespace LandmarkSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
