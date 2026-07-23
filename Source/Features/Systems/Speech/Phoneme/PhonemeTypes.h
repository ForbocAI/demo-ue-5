#pragma once

#include "Core/fp.hpp"

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

/** User Story: As a systems speech phoneme consumer, I need to compare values for equality through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool operator==(const FPhonemeEvent &Left, const FPhonemeEvent &Right) */
inline bool operator==(const FPhonemeEvent &Left,
                       const FPhonemeEvent &Right) {
  return Left.Phoneme == Right.Phoneme &&
         FMath::IsNearlyEqual(Left.StartTime, Right.StartTime) &&
         FMath::IsNearlyEqual(Left.Duration, Right.Duration);
}

/** User Story: As a systems speech phoneme consumer, I need to compare values for inequality through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool operator!=(const FPhonemeEvent &Left, const FPhonemeEvent &Right) */
inline bool operator!=(const FPhonemeEvent &Left,
                       const FPhonemeEvent &Right) {
  return !(Left == Right);
}

/** User Story: As a systems speech phoneme consumer, I need to compare values for equality through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool operator==(const FVisemeMapping &Left, const FVisemeMapping &Right) */
inline bool operator==(const FVisemeMapping &Left,
                       const FVisemeMapping &Right) {
  return Left.MorphTargetName == Right.MorphTargetName &&
         FMath::IsNearlyEqual(Left.BlendWeight, Right.BlendWeight);
}

/** User Story: As a systems speech phoneme consumer, I need to compare values for inequality through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool operator!=(const FVisemeMapping &Left, const FVisemeMapping &Right) */
inline bool operator!=(const FVisemeMapping &Left,
                       const FVisemeMapping &Right) {
  return !(Left == Right);
}
