#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Spatial/SpatialSelectors.h"
#include "Features/Systems/Interaction/InteractionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace InteractionReducers {

/**
 * @brief Maps JSON-backed interaction settings into the no-target player
 * message.
 *
 * @signature FString ReduceNoTownspersonMessage(const
 * ForbocAI::Demo::Data::FInteractionSettings &Settings)
 *
 * User story: As a content author, the player-facing missing-target message
 * can change in JSON without view logic or reducer constants.
 */
inline FString ReduceNoTownspersonMessage(
    const ForbocAI::Demo::Data::FInteractionSettings &Settings) {
  return Settings.NoTownspersonMessage;
}

/**
 * @brief Maps JSON-backed interaction range settings into world units.
 *
 * @signature float ReduceTownspersonInteractionDistance(const
 * FInteractionDistanceSettingsRequest &Request)
 *
 * User story: As a level designer, interaction range can be tuned in JSON
 * while RTK state remains the authority for gameplay distance.
 */
inline float ReduceTownspersonInteractionDistance(
    const FInteractionDistanceSettingsRequest &Request) {
  return SpatialSelectors::SelectTownLotWorldUnits(Request.Geometry) *
         Request.Interaction.TownspersonMaxDistanceLots;
}

inline FInteractionSelection ReduceEmptySelection(const FString &Message) {
  FInteractionSelection Selection;
  Selection.MissingMessage = Message;
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
    const FInteractionNearestCandidateRequest &Request,
    const FInteractionCandidate &Candidate,
    FInteractionSelection Current) {
  const float CandidateDistanceSquared =
      ReduceDistanceSquared(Request.Observation, Candidate);
  const bool bSelected =
      ReduceCandidateWithinRange(Request.Observation, Candidate) &&
      ReduceCandidateIsCloser(CandidateDistanceSquared, Current);
  Current.bFound = bSelected ? true : Current.bFound;
  Current.CandidateIndex =
      bSelected ? Candidate.Index : Current.CandidateIndex;
  Current.EntityId = bSelected ? Candidate.EntityId : Current.EntityId;
  Current.DistanceSquared =
      bSelected ? CandidateDistanceSquared : Current.DistanceSquared;
  Current.MissingMessage = Request.MissingMessage;
  return Current;
}

inline FInteractionSelection ReduceNearestCandidateRecursive(
    const FInteractionNearestCandidateRequest &Request, int32 Index,
    FInteractionSelection Current) {
  return Index >= Request.Observation.Candidates.Num()
             ? Current
             : ReduceNearestCandidateRecursive(
                   Request, Index + 1,
                   ReduceCloserCandidate(
                       Request, Request.Observation.Candidates[Index],
                       Current));
}

inline FInteractionSelection ReduceNearestCandidate(
    const FInteractionNearestCandidateRequest &Request) {
  return ReduceNearestCandidateRecursive(
      Request, 0, ReduceEmptySelection(Request.MissingMessage));
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
                ReduceNearestCandidate(
                    {Action.PayloadValue, Next.NoTownspersonMessage});
            return Next;
          })
      .val;
}

} // namespace InteractionReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
