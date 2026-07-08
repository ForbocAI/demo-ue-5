#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Systems/Interaction/SystemsInteractionTypes.h"

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
 * FDistanceSettingsRequest &Request)
 *
 * User story: As a level designer, interaction range can be tuned in JSON
 * while RTK state remains the authority for gameplay distance.
 */
inline float ReduceTownspersonInteractionDistance(
    const FDistanceSettingsRequest &Request) {
  return LevelLayoutAdapters::TownLotWorldUnits(Request.Geometry) *
         Request.Interaction.TownspersonMaxDistanceLots;
}

inline FSelection ReduceEmptySelection(const FString &Message) {
  FSelection Selection;
  Selection.MissingMessage = Message;
  return Selection;
}

inline float ReduceDistanceSquared(
    const FCandidatesObserved &Request,
    const FCandidate &Candidate) {
  return FVector::DistSquared(Request.Origin, Candidate.Location);
}

inline bool ReduceCandidateWithinRange(
    const FCandidatesObserved &Request,
    const FCandidate &Candidate) {
  return Candidate.bCanInteract &&
         ReduceDistanceSquared(Request, Candidate) <=
             FMath::Square(Request.MaxDistance);
}

inline bool ReduceCandidateIsCloser(float CandidateDistanceSquared,
                                    const FSelection &Current) {
  return !Current.bFound ||
         CandidateDistanceSquared < Current.DistanceSquared;
}

inline FSelection ReduceCloserCandidate(
    FSelection Current,
    const FNearestCandidateRequest &Request,
    const FCandidate &Candidate) {
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

inline FSelection ReduceNearestCandidate(
    const FNearestCandidateRequest &Request) {
  return func::fold_indexed(
      Request.Observation.Candidates,
      static_cast<size_t>(Request.Observation.Candidates.Num()),
      ReduceEmptySelection(Request.MissingMessage),
      [&Request](const FSelection &Current,
                 const FCandidate &Candidate) {
        return ReduceCloserCandidate(Current, Request, Candidate);
      });
}

/**
 * @brief Case reducer for InteractionActions::TownspersonCandidatesObserved.
 */
inline FInteractionState ReduceTownspersonCandidatesObserved(
    const FInteractionState &State,
    const rtk::PayloadAction<FCandidatesObserved> &Action) {
  return (func::pipe(State) |
          [&Action](FInteractionState Next) -> FInteractionState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.Origin = Action.PayloadValue.Origin;
            Next.MaxDistance = Action.PayloadValue.MaxDistance;
            Next.Candidates = Action.PayloadValue.Candidates;
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

#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Interaction/SystemsInteractionTypes.h"

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
            State.ActionId = func::nothing<FString>();
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
    const FCandidatesObserved &Payload) {
  return InteractionActions::TownspersonCandidatesObserved()(Payload);
}

} // namespace InteractionSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
