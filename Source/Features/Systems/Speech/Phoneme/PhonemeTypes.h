#pragma once

#include "Core/ue_fp.hpp"

#include "PhonemeTypes.generated.h"

USTRUCT(BlueprintType)
struct FPhonemeEvent {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  FString Phoneme;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float StartTime = 0.0f;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float Duration = 0.0f;
};

USTRUCT(BlueprintType)
struct FVisemeMapping {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speech")
  FString MorphTargetName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speech")
  float BlendWeight = 0.0f;
};

inline bool operator==(const FPhonemeEvent &Left,
                       const FPhonemeEvent &Right) {
  return Left.Phoneme == Right.Phoneme &&
         FMath::IsNearlyEqual(Left.StartTime, Right.StartTime) &&
         FMath::IsNearlyEqual(Left.Duration, Right.Duration);
}

inline bool operator!=(const FPhonemeEvent &Left,
                       const FPhonemeEvent &Right) {
  return !(Left == Right);
}

inline bool operator==(const FVisemeMapping &Left,
                       const FVisemeMapping &Right) {
  return Left.MorphTargetName == Right.MorphTargetName &&
         FMath::IsNearlyEqual(Left.BlendWeight, Right.BlendWeight);
}

inline bool operator!=(const FVisemeMapping &Left,
                       const FVisemeMapping &Right) {
  return !(Left == Right);
}
