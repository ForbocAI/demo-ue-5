#pragma once
/**
 * Pipeline — Deterministic tick execution for the BotOrchestrator.
 *
 * Structures the Tick function to execute systems in a deterministic order:
 *   Input → Logic → Physics → Output
 *
 * Each phase is a pure function:
 *   - Input:   Gather world queries (overlaps, visibility, movement targets)
 *   - Logic:   Evaluate systems, collect actions
 *   - Physics: Apply Tick action (time advancement, decay)
 *   - Output:  Dispatch all collected actions to the BotStore
 *
 * The pipeline processes one bot at a time, then moves to the next.
 * All bots in the orchestrator are processed in the same tick frame,
 * but each bot's pipeline is independent.
 *
 * User Story: As a game developer, I need a deterministic pipeline so
 * bot behavior is reproducible across frames and across sessions.
 */

#include "Features/Systems/Bots/Orchestrator/Factories/BotFactory.h"
#include "CoreMinimal.h"
#include "Features/Systems/Bots/Core/Actions.h"
#include "Features/Systems/Bots/Core/BotState.h"
#include "Features/Systems/Bots/Core/Reducers.h"
#include "Features/Systems/Bots/Pipeline/Systems.h"

namespace ForbocAI {
namespace Pipeline {

using namespace State;
using namespace Systems;

// ── Pipeline Phase Results ──

struct FInputPhaseResult {
  FWorldSnapshot WorldSnapshot;
};

struct FLogicPhaseResult {
  TArray<FBotAction> Actions;
};

struct FOutputPhaseResult {
  FBotState NewState;
  int32 ActionsDispatched;
};

// ── Input Phase ──

/**
 * Gathers world state into an immutable snapshot.
 *
 * In a full game, this would query UE collision, line traces, etc.
 * For the demo, we accept pre-built query results.
 *
 * Pure function: (DeltaTime, WorldQueries...) -> FInputPhaseResult
 */
inline FInputPhaseResult GatherInput(float DeltaTime,
                                      const FOverlapResult &Hazard,
                                      const FVisibilityResult &Vis,
                                      const FMovementQuery &Move) {
  FWorldSnapshot Snapshot;
  Snapshot.DeltaTime = DeltaTime;
  Snapshot.HazardOverlap = Hazard;
  Snapshot.Visibility = Vis;
  Snapshot.Movement = Move;
  Snapshot.Movement.DeltaTime = DeltaTime;
  return FInputPhaseResult{Snapshot};
}

/**
 * Default input — no hazards, no enemies, no movement goal.
 * Useful for idle ticks.
 */
inline FInputPhaseResult GatherDefaultInput(float DeltaTime) {
  return GatherInput(DeltaTime, FOverlapResult{}, FVisibilityResult{},
                     FMovementQuery{});
}

// ── Logic Phase ──

/**
 * Evaluates all systems and collects actions.
 *
 * Pure function: (BotState, WorldSnapshot) -> FLogicPhaseResult
 *
 * Always includes a Tick action first for time advancement.
 * Then appends system-generated actions.
 */
inline FLogicPhaseResult EvaluateLogic(const FBotState &State,
                                        const FWorldSnapshot &World) {
  TArray<FBotAction> Actions;

  // Always tick first (time advancement, memory decay)
  Actions.Add(FActionTick{World.DeltaTime});

  // Collect system actions
  const TArray<FBotAction> SystemActions =
      CollectSystemActions(State, World);

  // Append system actions (recursive)
  const auto AppendRecursive =
      [&Actions, &SystemActions](int32 Idx, const auto &Self) -> void {
    return Idx >= SystemActions.Num()
               ? void()
               : (Actions.Add(SystemActions[Idx]),
                  Self(Idx + 1, Self));
  };
  AppendRecursive(0, AppendRecursive);

  return FLogicPhaseResult{Actions};
}

// ── Physics Phase (Action Application) ──

/**
 * Applies all collected actions to the BotState through the reducer.
 *
 * Pure function: (BotState, Actions) -> FBotState
 *
 * Folds actions left-to-right through the reducer.
 * Each action produces a new state, which feeds into the next.
 *
 * This is the core "dispatch" step — equivalent to
 * store.dispatch(action) for each action in sequence.
 */
inline FBotState ApplyActions(const FBotState &State,
                               const TArray<FBotAction> &Actions,
                               int32 Idx = 0) {
  return Idx >= Actions.Num()
             ? State
             : ApplyActions(Reduce(State, Actions[Idx]), Actions, Idx + 1);
}

// ── Output Phase ──

/**
 * Finalizes the pipeline tick by producing the output result.
 *
 * Pure function: (BotState, Actions, NewState) -> FOutputPhaseResult
 */
inline FOutputPhaseResult ProduceOutput(const FBotState &NewState,
                                         int32 ActionCount) {
  return FOutputPhaseResult{NewState, ActionCount};
}

// ── Full Pipeline ──

/**
 * Runs the complete pipeline for a single bot in one tick.
 *
 * Phases:
 *   1. Input:   GatherInput (world queries → snapshot)
 *   2. Logic:   EvaluateLogic (systems → actions)
 *   3. Physics: ApplyActions (reducer fold)
 *   4. Output:  ProduceOutput (final state)
 *
 * Pure function: (BotState, WorldSnapshot) -> FOutputPhaseResult
 *
 * User Story: As the deterministic tick pipeline, I need all 4 phases
 * composed in order so each bot's behavior is a pure function of its
 * current state and the world snapshot.
 */
inline FOutputPhaseResult RunPipeline(const FBotState &CurrentState,
                                      const FWorldSnapshot &World) {
  // Phase 1: Input already gathered (World parameter)

  // Phase 2: Logic
  const FLogicPhaseResult Logic = EvaluateLogic(CurrentState, World);

  // Phase 3: Physics (reducer fold)
  const FBotState NewState = ApplyActions(CurrentState, Logic.Actions);

  // Phase 4: Output
  return ProduceOutput(NewState, Logic.Actions.Num());
}

/**
 * Runs the pipeline with default input (idle tick).
 */
inline FOutputPhaseResult RunIdlePipeline(const FBotState &CurrentState,
                                           float DeltaTime) {
  const FInputPhaseResult Input = GatherDefaultInput(DeltaTime);
  return RunPipeline(CurrentState, Input.WorldSnapshot);
}

// ── Multi-Bot Pipeline ──

/**
 * Process all bots in the orchestrator registry.
 *
 * Takes an array of (BotState, WorldSnapshot) pairs and runs the pipeline
 * for each. Returns an array of output results.
 *
 * Recursive — FP-compliant.
 */
struct FBotTickInput {
  FBotState State;
  FWorldSnapshot World;
};

inline TArray<FOutputPhaseResult>
RunMultiBotPipeline(const TArray<FBotTickInput> &Inputs, int32 Idx = 0,
                    TArray<FOutputPhaseResult> Acc = {}) {
  return Idx >= Inputs.Num()
             ? Acc
             : (Acc.Add(RunPipeline(Inputs[Idx].State, Inputs[Idx].World)),
                RunMultiBotPipeline(Inputs, Idx + 1, Acc));
}

} // namespace Pipeline
} // namespace ForbocAI
