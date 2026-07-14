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

inline bool operator==(const FBddSettings &Left,
                       const FBddSettings &Right) {
  return Left.Spec == Right.Spec && Left.Group == Right.Group &&
         Left.Case == Right.Case && Left.Assertion == Right.Assertion;
}

inline bool operator!=(const FBddSettings &Left,
                       const FBddSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Bdd == Right.Bdd;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Tests
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
