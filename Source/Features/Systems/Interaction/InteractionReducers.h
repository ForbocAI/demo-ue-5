#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Spatial/SpatialSelectors.h"
#include "Features/Systems/Interaction/InteractionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace InteractionReducers {

/**
 * @brief Pure reducer helper for the no-target player message.
 */
inline FString ReduceNoTownspersonMessage() {
  return TEXT("No talkable townsperson nearby.");
}

/**
 * @brief Pure reducer helper that derives the interaction radius from spatial
 * selectors.
 */
inline float ReduceTownspersonInteractionDistance() {
  return SpatialSelectors::SelectTownLotWorldUnits() * 2.1f;
}

inline FInteractionSelection ReduceEmptySelection() {
  FInteractionSelection Selection;
  Selection.MissingMessage = ReduceNoTownspersonMessage();
  return Selection;
}

inline float ReduceDistanceSquared(
    const FInteractionCandidatesObserved &Request,
    const FInteractionCandidate &Candidate) {
  return FVector::DistSquared(Request.Origin, Candidate.Location);
}

inline bool ReduceCandidateWithinRange(
    const FInteractionCandidatesObserved &Request,
    const FInteractionCandidate &Candidate) {
  return Candidate.bCanInteract &&
         ReduceDistanceSquared(Request, Candidate) <=
             FMath::Square(Request.MaxDistance);
}

inline bool ReduceCandidateIsCloser(float CandidateDistanceSquared,
                                    const FInteractionSelection &Current) {
  return !Current.bFound ||
         CandidateDistanceSquared < Current.DistanceSquared;
}

inline FInteractionSelection ReduceCloserCandidate(
    const FInteractionCandidatesObserved &Request,
    const FInteractionCandidate &Candidate,
    FInteractionSelection Current) {
  const float CandidateDistanceSquared =
      ReduceDistanceSquared(Request, Candidate);
  const bool bSelected =
      ReduceCandidateWithinRange(Request, Candidate) &&
      ReduceCandidateIsCloser(CandidateDistanceSquared, Current);
  Current.bFound = bSelected ? true : Current.bFound;
  Current.CandidateIndex =
      bSelected ? Candidate.Index : Current.CandidateIndex;
  Current.EntityId = bSelected ? Candidate.EntityId : Current.EntityId;
  Current.DistanceSquared =
      bSelected ? CandidateDistanceSquared : Current.DistanceSquared;
  Current.MissingMessage = ReduceNoTownspersonMessage();
  return Current;
}

inline FInteractionSelection ReduceNearestCandidateRecursive(
    const FInteractionCandidatesObserved &Request, int32 Index,
    FInteractionSelection Current) {
  return Index >= Request.Candidates.Num()
             ? Current
             : ReduceNearestCandidateRecursive(
                   Request, Index + 1,
                   ReduceCloserCandidate(Request, Request.Candidates[Index],
                                         Current));
}

inline FInteractionSelection ReduceNearestCandidate(
    const FInteractionCandidatesObserved &Request) {
  return ReduceNearestCandidateRecursive(Request, 0, ReduceEmptySelection());
}

/**
 * @brief Case reducer for InteractionActions::TownspersonCandidatesObserved.
 */
inline FInteractionState ReduceTownspersonCandidatesObserved(
    const FInteractionState &State,
    const rtk::PayloadAction<FInteractionCandidatesObserved> &Action) {
  return (func::pipe(State) |
          [&Action](FInteractionState Next) -> FInteractionState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.LastOrigin = Action.PayloadValue.Origin;
            Next.LastMaxDistance = Action.PayloadValue.MaxDistance;
            Next.LastCandidates = Action.PayloadValue.Candidates;
            Next.SelectedCandidate =
                ReduceNearestCandidate(Action.PayloadValue);
            return Next;
          })
      .val;
}

} // namespace InteractionReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
