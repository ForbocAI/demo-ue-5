#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Pipeline {

struct FSettings {
  FString Spec;
  TArray<FString> Tests;
  TArray<FString> Groups;
  TArray<FString> Cases;
  TArray<FString> Assertions;
};

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Tests == Right.Tests &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Pipeline
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
