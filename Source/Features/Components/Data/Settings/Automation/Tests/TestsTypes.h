#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Tests {

struct FBddSettings {
  FString Spec;
  FString Group;
  FString Case;
  FString Assertion;
};

struct FSettings {
  FBddSettings Bdd;
};

/** User Story: As a settings automation tests consumer, I need to compare values for equality through a stable signature so the settings automation tests workflow remains explicit and composable. @fn inline bool operator==(const FBddSettings &Left, const FBddSettings &Right) */
inline bool operator==(const FBddSettings &Left,
                       const FBddSettings &Right) {
  return Left.Spec == Right.Spec && Left.Group == Right.Group &&
         Left.Case == Right.Case && Left.Assertion == Right.Assertion;
}

/** User Story: As a settings automation tests consumer, I need to compare values for inequality through a stable signature so the settings automation tests workflow remains explicit and composable. @fn inline bool operator!=(const FBddSettings &Left, const FBddSettings &Right) */
inline bool operator!=(const FBddSettings &Left,
                       const FBddSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation tests consumer, I need to compare values for equality through a stable signature so the settings automation tests workflow remains explicit and composable. @fn inline bool operator==(const FSettings &Left, const FSettings &Right) */
inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Bdd == Right.Bdd;
}

/** User Story: As a settings automation tests consumer, I need to compare values for inequality through a stable signature so the settings automation tests workflow remains explicit and composable. @fn inline bool operator!=(const FSettings &Left, const FSettings &Right) */
inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Tests
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
