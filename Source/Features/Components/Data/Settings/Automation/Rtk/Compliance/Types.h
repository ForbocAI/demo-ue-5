#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Rtk {
namespace Compliance {

struct FPattern {
  FString Token;
  FString Message;
};

struct FSettings {
  TArray<FString> SourceFileSuffixes;
  TArray<FString> AllowedBoundaryFragments;
  FString SourceDirectoryName;
  FString SourceSearchPattern;
  TArray<FPattern> ForbiddenPatterns;
  FString ViolationMessageFormat;
  FString SourceReadFailureFormat;
  FString StoreBoundaryLabel;
  int32 ViolationCountIncrement;
  int32 CleanViolationCount;
};

inline bool operator==(const FPattern &Left, const FPattern &Right) {
  return Left.Token == Right.Token && Left.Message == Right.Message;
}

inline bool operator!=(const FPattern &Left, const FPattern &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.SourceFileSuffixes == Right.SourceFileSuffixes &&
         Left.AllowedBoundaryFragments == Right.AllowedBoundaryFragments &&
         Left.SourceDirectoryName == Right.SourceDirectoryName &&
         Left.SourceSearchPattern == Right.SourceSearchPattern &&
         Left.ForbiddenPatterns == Right.ForbiddenPatterns &&
         Left.ViolationMessageFormat == Right.ViolationMessageFormat &&
         Left.SourceReadFailureFormat == Right.SourceReadFailureFormat &&
         Left.StoreBoundaryLabel == Right.StoreBoundaryLabel &&
         Left.ViolationCountIncrement == Right.ViolationCountIncrement &&
         Left.CleanViolationCount == Right.CleanViolationCount;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Compliance
} // namespace Rtk
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
