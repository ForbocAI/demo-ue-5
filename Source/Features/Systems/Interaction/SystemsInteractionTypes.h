#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

/**
 * @brief View-observed candidate lowered into store-owned interaction data.
 */
struct FCandidate {
  int32 Index = INDEX_NONE;
  FString EntityId;
  FVector Location = FVector::ZeroVector;
  bool bCanInteract = false;
};

/**
 * @brief RTK payload for one candidate-observation event.
 *
 * Architecture: UE boundary code can observe locations, but the reducer owns
 * nearest-candidate selection and missing-interaction messaging.
 */
struct FCandidatesObserved {
  FString Id;
  FVector Origin = FVector::ZeroVector;
  float MaxDistance = 0.0f;
  TArray<FCandidate> Candidates;
};

/**
 * @brief Reducer request for selecting a nearest candidate with configured
 * empty-state messaging.
 */
struct FNearestCandidateRequest {
  FCandidatesObserved Observation;
  FString MissingMessage;
};

struct FDistanceSettingsRequest {
  ForbocAI::Game::Data::FInteractionSettings Interaction;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

/**
 * @brief Reducer-owned result of selecting an interaction target.
 */
struct FSelection {
  bool bFound = false;
  int32 CandidateIndex = INDEX_NONE;
  FString EntityId;
  float DistanceSquared = 0.0f;
  FString MissingMessage;
};

/**
 * @brief Interaction slice state in the single runtime store.
 */
struct FInteractionState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  FVector Origin = FVector::ZeroVector;
  float MaxDistance = 0.0f;
  float TownspersonMaxDistance = 0.0f;
  FString NoTownspersonMessage;
  TArray<FCandidate> Candidates;
  FSelection SelectedCandidate;
};

inline bool operator==(const FCandidate &Left,
                       const FCandidate &Right) {
  return Left.Index == Right.Index && Left.EntityId == Right.EntityId &&
         Left.Location == Right.Location &&
         Left.bCanInteract == Right.bCanInteract;
}

inline bool operator!=(const FCandidate &Left,
                       const FCandidate &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCandidatesObserved &Left,
                       const FCandidatesObserved &Right) {
  return Left.Id == Right.Id && Left.Origin == Right.Origin &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance) &&
         Left.Candidates == Right.Candidates;
}

inline bool operator!=(const FCandidatesObserved &Left,
                       const FCandidatesObserved &Right) {
  return !(Left == Right);
}

inline bool operator==(const FNearestCandidateRequest &Left,
                       const FNearestCandidateRequest &Right) {
  return Left.Observation == Right.Observation &&
         Left.MissingMessage == Right.MissingMessage;
}

inline bool operator!=(const FNearestCandidateRequest &Left,
                       const FNearestCandidateRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSelection &Left,
                       const FSelection &Right) {
  return Left.bFound == Right.bFound &&
         Left.CandidateIndex == Right.CandidateIndex &&
         Left.EntityId == Right.EntityId &&
         FMath::IsNearlyEqual(Left.DistanceSquared, Right.DistanceSquared) &&
         Left.MissingMessage == Right.MissingMessage;
}

inline bool operator!=(const FSelection &Left,
                       const FSelection &Right) {
  return !(Left == Right);
}

inline bool operator==(const FInteractionState &Left,
                       const FInteractionState &Right) {
  return Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value) &&
         Left.Origin == Right.Origin &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance) &&
         FMath::IsNearlyEqual(Left.TownspersonMaxDistance,
                              Right.TownspersonMaxDistance) &&
         Left.NoTownspersonMessage == Right.NoTownspersonMessage &&
         Left.Candidates == Right.Candidates &&
         Left.SelectedCandidate == Right.SelectedCandidate;
}

inline bool operator!=(const FInteractionState &Left,
                       const FInteractionState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
