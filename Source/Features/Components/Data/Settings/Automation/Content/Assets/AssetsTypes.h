#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Content {
namespace Assets {

struct FPackage {
  FString Label;
  FString Path;
};

struct FConfig {
  FString Label;
  FString Section;
  FString Key;
  FString Expected;
};

struct FSettings {
  TArray<FPackage> Packages;
  TArray<FPackage> Classes;
  TArray<FPackage> SpeechComponentClasses;
  TArray<FConfig> ConfigValues;
  TArray<FPackage> Assets;
  TArray<FPackage> SkeletalMeshLods;
  TArray<FPackage> MissingPackages;
  FString SkeletalMeshLoadsLabelFormat;
  FString SkeletalMeshLodDataLabelFormat;
  FString NativeLodAuditCountFormat;
  FString NativeLodAuditEntryFormat;
};

inline bool operator==(const FPackage &Left, const FPackage &Right) {
  return Left.Label == Right.Label && Left.Path == Right.Path;
}

inline bool operator!=(const FPackage &Left, const FPackage &Right) {
  return !(Left == Right);
}

inline bool operator==(const FConfig &Left, const FConfig &Right) {
  return Left.Label == Right.Label && Left.Section == Right.Section &&
         Left.Key == Right.Key && Left.Expected == Right.Expected;
}

inline bool operator!=(const FConfig &Left, const FConfig &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Packages == Right.Packages &&
         Left.Classes == Right.Classes &&
         Left.SpeechComponentClasses == Right.SpeechComponentClasses &&
         Left.ConfigValues == Right.ConfigValues &&
         Left.Assets == Right.Assets &&
         Left.SkeletalMeshLods == Right.SkeletalMeshLods &&
         Left.MissingPackages == Right.MissingPackages &&
         Left.SkeletalMeshLoadsLabelFormat ==
             Right.SkeletalMeshLoadsLabelFormat &&
         Left.SkeletalMeshLodDataLabelFormat ==
             Right.SkeletalMeshLodDataLabelFormat &&
         Left.NativeLodAuditCountFormat == Right.NativeLodAuditCountFormat &&
         Left.NativeLodAuditEntryFormat == Right.NativeLodAuditEntryFormat;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Assets
} // namespace Content
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
