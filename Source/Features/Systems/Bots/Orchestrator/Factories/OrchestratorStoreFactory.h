#pragma once

#include "Features/Systems/Bots/Orchestrator/Factories/BotFactory.h"
#include "CoreMinimal.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "NPC/NPCModule.h"
#endif

#include <functional>
#include <memory>

/**
 * Orchestrator Store Factory — Closure-based registry for multi-bot management.
 *
 * Replaces the mutable TMap<AActor*, FBotInstance> with a functional store
 * that follows the same pattern as BotFactory::CreateBotStore:
 *   - State is held in a shared_ptr captured by closures
 *   - Dispatch replaces state via copy-on-write
 *   - GetState returns an immutable snapshot
 *
 * User Story: As a maintainer, I need the orchestrator registry to follow
 * the same closure-based store pattern as the individual bot stores so
 * the entire bot management layer is FP-compliant.
 */

namespace ForbocAI {
namespace Orchestrator {

// ── Bot Instance (unchanged) ──

struct FBotInstance {
  AActor *BotActor;
#if WITH_FORBOC_AI_SDK_DEMO
  TSharedPtr<const FAgent> Agent;
#endif
  Bot::FBotStore Store;
  float LastObservationTime;

  FBotInstance()
      : BotActor(nullptr),
#if WITH_FORBOC_AI_SDK_DEMO
        Agent(nullptr),
#endif
        Store({}),
        LastObservationTime(0.0f) {}
};

// ── Registry State ──
// The immutable snapshot of all registered bots.

struct FRegistryState {
  TMap<AActor *, FBotInstance> Bots;
  int32 TotalRegistered;

  FRegistryState() : TotalRegistered(0) {}
};

// ── Actions ──

struct FRegisterBotAction {
  AActor *Actor;
  FBotInstance Instance;
};

struct FUnregisterBotAction {
  AActor *Actor;
};

struct FUpdateInstanceAction {
  AActor *Actor;
  FBotInstance UpdatedInstance;
};

using FRegistryAction =
    TVariant<FRegisterBotAction, FUnregisterBotAction, FUpdateInstanceAction>;

// ── Reducer ──

namespace detail {

inline FRegistryState ReduceRegister(const FRegistryState &State,
                                      const FRegisterBotAction &Action) {
  FRegistryState Next = State;
  Next.Bots.Add(Action.Actor, Action.Instance);
  Next.TotalRegistered = State.TotalRegistered + 1;
  return Next;
}

inline FRegistryState ReduceUnregister(const FRegistryState &State,
                                        const FUnregisterBotAction &Action) {
  FRegistryState Next = State;
  Next.Bots.Remove(Action.Actor);
  return Next;
}

inline FRegistryState ReduceUpdate(const FRegistryState &State,
                                    const FUpdateInstanceAction &Action) {
  FRegistryState Next = State;
  Next.Bots.Add(Action.Actor, Action.UpdatedInstance);
  return Next;
}

} // namespace detail

inline FRegistryState ReduceRegistry(const FRegistryState &State,
                                      const FRegistryAction &Action) {
  // Visitor dispatch (FP-compliant, no switch)
  return Visit(
      [&State](auto &&Arg) -> FRegistryState {
        using T = std::decay_t<decltype(Arg)>;
        return std::is_same<T, FRegisterBotAction>::value
                   ? detail::ReduceRegister(
                         State,
                         *reinterpret_cast<const FRegisterBotAction *>(&Arg))
               : std::is_same<T, FUnregisterBotAction>::value
                   ? detail::ReduceUnregister(
                         State,
                         *reinterpret_cast<const FUnregisterBotAction *>(&Arg))
               : detail::ReduceUpdate(
                     State,
                     *reinterpret_cast<const FUpdateInstanceAction *>(&Arg));
      },
      Action);
}

// ── Store Type ──

using RegistryDispatcher =
    std::function<FRegistryState(FRegistryAction)>;
using RegistryGetter = std::function<FRegistryState()>;

struct FOrchestratorStore {
  RegistryDispatcher Dispatch;
  RegistryGetter GetState;
};

// ── Factory ──

namespace Factory {

/**
 * Creates a closure-based orchestrator store.
 * State is captured in a shared_ptr — same pattern as BotFactory.
 *
 * User Story: As the bot orchestration layer, I need a functional store
 * so bot registration, unregistration, and updates follow copy-on-write
 * semantics without mutable class members.
 */
inline FOrchestratorStore CreateOrchestratorStore() {
  auto StateContainer =
      std::make_shared<FRegistryState>(FRegistryState{});

  RegistryDispatcher Dispatch =
      [StateContainer](FRegistryAction Action) -> FRegistryState {
    FRegistryState NewState = ReduceRegistry(*StateContainer, Action);
    *StateContainer = NewState;
    return NewState;
  };

  RegistryGetter GetState = [StateContainer]() -> FRegistryState {
    return *StateContainer;
  };

  return {Dispatch, GetState};
}

} // namespace Factory

} // namespace Orchestrator
} // namespace ForbocAI
