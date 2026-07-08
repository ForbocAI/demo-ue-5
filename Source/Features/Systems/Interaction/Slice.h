#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Components/Data/Types.h"
#include "Features/Components/Spatial/Level/Layout/Adapters.h"
#include "Features/Systems/Interaction/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionReducers {

/**
 * @brief Maps JSON-backed interaction settings into the no-target player
 * message.
 *
 * @signature FString ReduceNoTownspersonMessage(const
 * ForbocAI::Game::Data::FInteractionSettings &Settings)
 *
 * User story: As a content author, the player-facing missing-target message
 * can change in JSON without view logic or reducer constants.
 */
inline FString ReduceNoTownspersonMessage(
    const ForbocAI::Game::Data::FInteractionSettings &Settings) {
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
  return LevelLayoutAdapters::TownLotWorldUnits(Request.Geometry) *
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
    FInteractionSelection Current,
    const FInteractionNearestCandidateRequest &Request,
    const FInteractionCandidate &Candidate) {
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

inline FInteractionSelection ReduceNearestCandidate(
    const FInteractionNearestCandidateRequest &Request) {
  return func::fold_indexed(
      Request.Observation.Candidates,
      static_cast<size_t>(Request.Observation.Candidates.Num()),
      ReduceEmptySelection(Request.MissingMessage),
      [&Request](const FInteractionSelection &Current,
                 const FInteractionCandidate &Candidate) {
        return ReduceCloserCandidate(Current, Request, Candidate);
      });
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
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Actions.h"
#include "Features/Systems/Interaction/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionSlice {

/**
 * @brief Creates initial state for the interaction RTK slice.
 */
inline FInteractionState CreateInitialState() {
  return (func::pipe(FInteractionState{}) |
          [](FInteractionState State) -> FInteractionState {
            State.LastActionId = func::nothing<FString>();
            return State;
          })
      .val;
}

/**
 * @brief Returns the interaction slice binding actions to case reducers.
 */
inline const rtk::Slice<FInteractionState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FInteractionState>> Slice =
      func::lazy([]() -> rtk::Slice<FInteractionState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FInteractionState>(
            TEXT("systems/interaction"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FInteractionState> &Builder) {
              Builder.addCase(
                  InteractionActions::TownspersonCandidatesObserved(),
                  InteractionReducers::ReduceTownspersonCandidatesObserved);
            });
      });
  return func::eval(Slice);
}

inline rtk::AnyAction
TownspersonCandidatesObserved(
    const FInteractionCandidatesObserved &Payload) {
  return InteractionActions::TownspersonCandidatesObserved()(Payload);
}

} // namespace InteractionSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
