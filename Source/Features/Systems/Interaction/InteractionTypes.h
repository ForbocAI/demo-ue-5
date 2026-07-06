#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

/**
 * @brief View-observed candidate lowered into store-owned interaction data.
 */
struct FInteractionCandidate {
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
struct FInteractionCandidatesObserved {
  FString Id;
  FVector Origin = FVector::ZeroVector;
  float MaxDistance = 0.0f;
  TArray<FInteractionCandidate> Candidates;
};

/**
 * @brief Reducer request for selecting a nearest candidate with configured
 * empty-state messaging.
 */
struct FInteractionNearestCandidateRequest {
  FInteractionCandidatesObserved Observation;
  FString MissingMessage;
};

struct FInteractionDistanceSettingsRequest {
  ForbocAI::Game::Data::FInteractionSettings Interaction;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

/**
 * @brief Reducer-owned result of selecting an interaction target.
 */
struct FInteractionSelection {
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
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  FVector LastOrigin = FVector::ZeroVector;
  float LastMaxDistance = 0.0f;
  float TownspersonMaxDistance = 0.0f;
  FString NoTownspersonMessage;
  TArray<FInteractionCandidate> LastCandidates;
  FInteractionSelection SelectedCandidate;
};

inline bool operator==(const FInteractionCandidate &Left,
                       const FInteractionCandidate &Right) {
  return Left.Index == Right.Index && Left.EntityId == Right.EntityId &&
         Left.Location == Right.Location &&
         Left.bCanInteract == Right.bCanInteract;
}

inline bool operator!=(const FInteractionCandidate &Left,
                       const FInteractionCandidate &Right) {
  return !(Left == Right);
}

inline bool operator==(const FInteractionCandidatesObserved &Left,
                       const FInteractionCandidatesObserved &Right) {
  return Left.Id == Right.Id && Left.Origin == Right.Origin &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance) &&
         Left.Candidates == Right.Candidates;
}

inline bool operator!=(const FInteractionCandidatesObserved &Left,
                       const FInteractionCandidatesObserved &Right) {
  return !(Left == Right);
}

inline bool operator==(const FInteractionNearestCandidateRequest &Left,
                       const FInteractionNearestCandidateRequest &Right) {
  return Left.Observation == Right.Observation &&
         Left.MissingMessage == Right.MissingMessage;
}

inline bool operator!=(const FInteractionNearestCandidateRequest &Left,
                       const FInteractionNearestCandidateRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FInteractionSelection &Left,
                       const FInteractionSelection &Right) {
  return Left.bFound == Right.bFound &&
         Left.CandidateIndex == Right.CandidateIndex &&
         Left.EntityId == Right.EntityId &&
         FMath::IsNearlyEqual(Left.DistanceSquared, Right.DistanceSquared) &&
         Left.MissingMessage == Right.MissingMessage;
}

inline bool operator!=(const FInteractionSelection &Left,
                       const FInteractionSelection &Right) {
  return !(Left == Right);
}

inline bool operator==(const FInteractionState &Left,
                       const FInteractionState &Right) {
  return Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value) &&
         Left.LastOrigin == Right.LastOrigin &&
         FMath::IsNearlyEqual(Left.LastMaxDistance, Right.LastMaxDistance) &&
         FMath::IsNearlyEqual(Left.TownspersonMaxDistance,
                              Right.TownspersonMaxDistance) &&
         Left.NoTownspersonMessage == Right.NoTownspersonMessage &&
         Left.LastCandidates == Right.LastCandidates &&
         Left.SelectedCandidate == Right.SelectedCandidate;
}

inline bool operator!=(const FInteractionState &Left,
                       const FInteractionState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
