#pragma once
/**
 * Systems — Pure query-and-dispatch functions that bridge world state
 * to BotStore actions.
 *
 * Each system is a pure function:
 *   (BotState, WorldQuery) -> Maybe<FBotAction>
 *
 * Systems DO NOT mutate state. They read the current BotState snapshot,
 * evaluate world conditions, and return an action to dispatch (or nothing).
 *
 * Design:
 *   - HazardSystem: Overlap queries → TakeDamage actions
 *   - MovementSystem: Position interpolation → UpdatePosition actions
 *   - AwarenessSystem: Line-of-sight queries → SpotEnemy actions
 *   - PhaseSystem: Phase transitions based on current state
 *
 * User Story: As a game developer, I need a systems layer that queries
 * BotState and produces actions, following the ECS-like pattern where
 * systems are pure functions over state.
 */

#include "CoreMinimal.h"
#include "State/Actions.h"
#include "State/BotState.h"

namespace ForbocAI {
namespace Systems {

using namespace State;

// ── World Query Inputs (immutable snapshots of world state) ──

struct FOverlapResult {
  bool bOverlapping;
  float DamagePerSecond;
  AActor *HazardSource;

  FOverlapResult()
      : bOverlapping(false), DamagePerSecond(0.0f), HazardSource(nullptr) {}
};

struct FVisibilityResult {
  bool bCanSeeEnemy;
  FVector EnemyPosition;
  float Distance;

  FVisibilityResult()
      : bCanSeeEnemy(false), EnemyPosition(FVector::ZeroVector),
        Distance(0.0f) {}
};

struct FMovementQuery {
  FVector TargetPosition;
  float DeltaTime;
  float InterpSpeed;

  FMovementQuery()
      : TargetPosition(FVector::ZeroVector), DeltaTime(0.0f),
        InterpSpeed(300.0f) {}
};

// ── Hazard System ──

/**
 * Evaluates hazard overlaps and produces a TakeDamage action if applicable.
 *
 * Pure function: (BotState, OverlapResult, DeltaTime) -> Maybe<FBotAction>
 *
 * Produces a TakeDamage action when:
 *   - The bot is overlapping a hazard zone
 *   - The bot is still alive (Health > 0)
 *
 * Returns nothing when:
 *   - No overlap
 *   - Bot is already dead
 */
inline func::Maybe<FBotAction>
EvaluateHazards(const FBotState &State, const FOverlapResult &Overlap,
                float DeltaTime) {
  if (!Overlap.bOverlapping || State.Stats.Health <= 0.0f) {
    return func::nothing<FBotAction>();
  }

  return func::just<FBotAction>(FBotAction{
      FActionTakeDamage{Overlap.DamagePerSecond * DeltaTime,
                        Overlap.HazardSource}});
}

// ── Movement System ──

/**
 * Evaluates movement interpolation and produces a Move action if needed.
 *
 * Pure function: (BotState, MovementQuery) -> Maybe<FBotAction>
 *
 * Produces a Move action when:
 *   - The bot has a valid target position different from current
 *   - The bot is alive
 *   - The bot is not fleeing (flee uses a separate flee vector)
 *
 * The interpolated position is calculated as a lerp toward the target.
 * The actual pawn movement is handled by the Actuator, not this system.
 */
inline func::Maybe<FBotAction>
EvaluateMovement(const FBotState &State, const FMovementQuery &Query) {
  const float DistSq =
      FVector::DistSquared(State.Position, Query.TargetPosition);

  // Already at target (within 1 unit) or dead
  if (DistSq < 1.0f || State.Stats.Health <= 0.0f) {
    return func::nothing<FBotAction>();
  }

  return func::just<FBotAction>(
      FBotAction{FActionMove{FMath::VInterpConstantTo(
                                 State.Position, Query.TargetPosition,
                                 Query.DeltaTime, Query.InterpSpeed),
                             Query.InterpSpeed}});
}

// ── Awareness System ──

/**
 * Evaluates line-of-sight visibility and produces SpotEnemy if applicable.
 *
 * Pure function: (BotState, VisibilityResult) -> Maybe<FBotAction>
 *
 * Produces a SpotEnemy action when:
 *   - The bot can see an enemy
 *   - The bot doesn't already have aggro on that position
 *   - The bot is alive
 *
 * Returns nothing when:
 *   - No enemy visible
 *   - Bot already has aggro (avoids redundant dispatches)
 */
inline func::Maybe<FBotAction>
EvaluateAwareness(const FBotState &State, const FVisibilityResult &Vis) {
  if (!Vis.bCanSeeEnemy || State.Stats.Health <= 0.0f) {
    return func::nothing<FBotAction>();
  }

  if (State.Memory.bHasAggro &&
      FVector::DistSquared(State.Memory.LastKnownPlayerPos,
                           Vis.EnemyPosition) < 100.0f) {
    return func::nothing<FBotAction>();
  }

  return func::just<FBotAction>(
      FBotAction{FActionSpotEnemy{Vis.EnemyPosition}});
}

// ── Phase System ──

/**
 * Evaluates phase transitions based on current state.
 *
 * Pure function: (BotState) -> Maybe<FBotAction>
 *
 * Produces a Flee action when:
 *   - Bot is in Combat phase but health is critically low
 *
 * Returns nothing when:
 *   - No phase transition needed
 */
inline func::Maybe<FBotAction>
EvaluatePhaseTransition(const FBotState &State) {
  // Flee threshold: 20% health in combat
  if (State.Phase == EBotPhase::Combat &&
      State.Stats.Health < State.Stats.MaxHealth * 0.2f &&
      State.Memory.bHasAggro) {
    return func::just<FBotAction>(
        FBotAction{FActionFlee{State.Memory.LastKnownPlayerPos}});
  }

  return func::nothing<FBotAction>();
}

// ── System Registry (typed list for pipeline) ──

/**
 * A system is a pure function: (BotState, ...) -> Maybe<FBotAction>
 *
 * We wrap them in a uniform signature for pipeline execution.
 * Each evaluator takes the BotState and a generic "world context" and
 * returns a list of actions to dispatch.
 */
struct FWorldSnapshot {
  FOverlapResult HazardOverlap;
  FVisibilityResult Visibility;
  FMovementQuery Movement;
  float DeltaTime;

  FWorldSnapshot() : DeltaTime(0.0f) {}
};

/**
 * Collect all system evaluations into a list of actions.
 *
 * Pure function: (BotState, WorldSnapshot) -> TArray<FBotAction>
 *
 * Evaluates systems in deterministic order:
 *   1. Hazard (damage first — may kill the bot)
 *   2. Awareness (update threat info)
 *   3. Phase (transition based on new state)
 *   4. Movement (move toward goal)
 *
 * User Story: As the deterministic pipeline, I need all systems
 * evaluated in a fixed order so state transitions are reproducible.
 */
inline TArray<FBotAction>
CollectSystemActions(const FBotState &State,
                     const FWorldSnapshot &World) {
  TArray<FBotAction> Actions;

  // 1. Hazards
  const auto Hazard = EvaluateHazards(State, World.HazardOverlap,
                                       World.DeltaTime);
  Hazard.hasValue ? (Actions.Add(Hazard.value), void()) : void();

  // 2. Awareness
  const auto Awareness = EvaluateAwareness(State, World.Visibility);
  Awareness.hasValue ? (Actions.Add(Awareness.value), void()) : void();

  // 3. Phase transition
  const auto Phase = EvaluatePhaseTransition(State);
  Phase.hasValue ? (Actions.Add(Phase.value), void()) : void();

  // 4. Movement
  const auto Movement = EvaluateMovement(State, World.Movement);
  Movement.hasValue ? (Actions.Add(Movement.value), void()) : void();

  return Actions;
}

} // namespace Systems
} // namespace ForbocAI
