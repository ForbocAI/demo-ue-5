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

struct FSource {
  TArray<FString> SourceFileSuffixes;
  TArray<FString> AllowedBoundaryFragments;
  FString SourceDirectoryName;
  FString SourceSearchPattern;
};

struct FViolation {
  FString ViolationMessageFormat;
  FString SourceReadFailureFormat;
  int32 ViolationCountIncrement;
  int32 CleanViolationCount;
};

struct FStoreBoundary {
  FString StoreBoundaryLabel;
};

struct FSettings {
  FSource Source;
  TArray<FPattern> ForbiddenPatterns;
  FViolation Violation;
  FStoreBoundary StoreBoundary;
};

inline bool operator==(const FPattern &Left, const FPattern &Right) {
  return Left.Token == Right.Token && Left.Message == Right.Message;
}

inline bool operator!=(const FPattern &Left, const FPattern &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSource &Left, const FSource &Right) {
  return Left.SourceFileSuffixes == Right.SourceFileSuffixes &&
         Left.AllowedBoundaryFragments == Right.AllowedBoundaryFragments &&
         Left.SourceDirectoryName == Right.SourceDirectoryName &&
         Left.SourceSearchPattern == Right.SourceSearchPattern;
}

inline bool operator!=(const FSource &Left, const FSource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FViolation &Left, const FViolation &Right) {
  return Left.ViolationMessageFormat == Right.ViolationMessageFormat &&
         Left.SourceReadFailureFormat == Right.SourceReadFailureFormat &&
         Left.ViolationCountIncrement == Right.ViolationCountIncrement &&
         Left.CleanViolationCount == Right.CleanViolationCount;
}

inline bool operator!=(const FViolation &Left, const FViolation &Right) {
  return !(Left == Right);
}

inline bool operator==(const FStoreBoundary &Left,
                       const FStoreBoundary &Right) {
  return Left.StoreBoundaryLabel == Right.StoreBoundaryLabel;
}

inline bool operator!=(const FStoreBoundary &Left,
                       const FStoreBoundary &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Source == Right.Source &&
         Left.ForbiddenPatterns == Right.ForbiddenPatterns &&
         Left.Violation == Right.Violation &&
         Left.StoreBoundary == Right.StoreBoundary;
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
