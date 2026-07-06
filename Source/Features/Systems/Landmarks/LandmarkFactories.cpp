#include "Features/Systems/Landmarks/LandmarkFactories.h"

#include "Features/Systems/Landmarks/LandmarkAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkFactories {

FLandmarkState CreateInitialState() {
  FLandmarkState State;
  State.Items = LandmarkAdapters::LandmarkAdapter().getInitialState();
  return State;
}

FLandmark Landmark(const FLandmarkSource &Source) {
  FLandmark Result;
  Result.Id = Source.Id;
  Result.Label = Source.Label;
  Result.Kind = Source.Kind;
  Result.Location = Source.Location;
  Result.Scale = Source.Scale;
  return Result;
}

} // namespace LandmarkFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
