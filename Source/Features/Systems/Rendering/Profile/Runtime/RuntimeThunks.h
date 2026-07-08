#pragma once

#include "CoreMinimal.h"

#include "Core/ue_fp.hpp"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"
#include "Features/Systems/Rendering/Profile/Color/ColorTypes.h"
#include "GameFramework/Actor.h"
#include "HAL/IConsoleManager.h"

#include <functional>

// Runtime render profile logging is shared across every profile subdomain
// translation unit, so the category is declared here and defined once in the
// orchestrator (RenderingProfileThunks.cpp).
DECLARE_LOG_CATEGORY_EXTERN(LogForbocRenderingProfile, Log, All);

namespace ForbocAI {
namespace Game {
namespace Data {
struct FRenderingSettings;
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {

using RenderingProfileColorTypes::FProfileColorChannels;
using RenderingProfileColorTypes::ProfileLinearColor;

// One world+profile transition payload shared by every runtime profile
// subdomain. Replaces the former per-subdomain FRetroRuntime*Eval family, all
// of which carried the identical two fields.
struct FRuntimeProfileEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

// --- Runtime console variable writes -------------------------------------
inline func::Maybe<IConsoleVariable *>
FindRuntimeProfileConsoleVariable(const FString &Name) {
  IConsoleVariable *Found = IConsoleManager::Get().FindConsoleVariable(*Name);
  return func::from_nullable_value<IConsoleVariable *>(Found, Found != nullptr);
}

template <typename Value>
bool ApplyRuntimeConsoleVariableValue(const FString &Name, Value NewValue) {
  return func::match(
      FindRuntimeProfileConsoleVariable(Name),
      [NewValue](IConsoleVariable *Found) {
        Found->Set(NewValue, ECVF_SetByGameSetting);
        return true;
      },
      [&Name]() {
        UE_LOG(LogForbocRenderingProfile, Warning,
               TEXT("Rendering console variable '%s' is unavailable; "
                    "runtime profile value skipped."),
               *Name);
        return false;
      });
}

inline void SetRuntimeCVarFloat(const FString &Name, float Value) {
  ApplyRuntimeConsoleVariableValue<float>(Name, Value);
}

inline void SetRuntimeCVarInt(const FString &Name, int32 Value) {
  ApplyRuntimeConsoleVariableValue<int32>(Name, Value);
}

// --- Actor selection ------------------------------------------------------
template <typename Actor>
TArray<Actor *> CollectRuntimeProfileActors(TActorIterator<Actor> &Iterator,
                                            TArray<Actor *> Actors) {
  return Iterator ? (Actors.Add(*Iterator), ++Iterator,
                     CollectRuntimeProfileActors<Actor>(Iterator, Actors))
                  : Actors;
}

template <typename Actor>
TArray<Actor *> SelectRuntimeProfileActors(UWorld *World) {
  TActorIterator<Actor> Iterator(World);
  return CollectRuntimeProfileActors<Actor>(Iterator, {});
}

template <typename Actor>
Actor *SelectTaggedRuntimeProfileActor(UWorld *World, const FName &Tag) {
  return func::match(
      func::find_array<Actor *>(
          SelectRuntimeProfileActors<Actor>(World),
          [&Tag](Actor *Candidate) {
            return Candidate != nullptr && Candidate->ActorHasTag(Tag);
          }),
      [](Actor *Found) { return Found; }, []() { return nullptr; });
}

template <typename Actor>
Actor *SelectOrSpawnTaggedRuntimeProfileActor(UWorld *World, const FName &Tag) {
  Actor *Existing = SelectTaggedRuntimeProfileActor<Actor>(World, Tag);
  Actor *Selected =
      Existing != nullptr
          ? Existing
          : World->SpawnActor<Actor>(FVector::ZeroVector, FRotator::ZeroRotator);
  Selected != nullptr ? (Selected->Tags.AddUnique(Tag), void()) : void();
  return Selected;
}

// --- World-guarded apply runners -----------------------------------------
// The world-null guard, actor selection/spawn, and per-actor traversal live
// once here. Subdomains supply only the actor type and the irreducible apply
// behavior, and receive a unary function over FRuntimeProfileEval (a factory
// that returns a function, per the ue_fp cookbook).
template <typename Effect>
void WithRuntimeWorld(UWorld *World, Effect RunWithWorld) {
  func::match(func::from_nullable_value(World, World != nullptr),
              [&RunWithWorld](UWorld *WorldValue) { RunWithWorld(WorldValue); },
              []() {});
}

template <typename Actor, typename Apply>
std::function<void(const FRuntimeProfileEval &)>
ApplyToTaggedProfileActor(const FName &Tag, Apply ApplyActor) {
  return [Tag, ApplyActor](const FRuntimeProfileEval &Eval) {
    WithRuntimeWorld(Eval.World, [&](UWorld *WorldValue) {
      Actor *Selected =
          SelectOrSpawnTaggedRuntimeProfileActor<Actor>(WorldValue, Tag);
      Selected ? (ApplyActor(Selected, Eval.Profile), void()) : void();
    });
  };
}

template <typename Actor, typename Apply>
std::function<void(const FRuntimeProfileEval &)>
ApplyToEachProfileActorOrSpawn(Apply ApplyActor) {
  return [ApplyActor](const FRuntimeProfileEval &Eval) {
    WithRuntimeWorld(Eval.World, [&](UWorld *WorldValue) {
      const TArray<Actor *> Existing =
          SelectRuntimeProfileActors<Actor>(WorldValue);
      func::for_each_array<Actor *>(
          Existing, [&](Actor *const &Candidate) {
            Candidate ? (ApplyActor(Candidate, Eval.Profile), void()) : void();
          });
      Actor *Spawned =
          Existing.IsEmpty()
              ? WorldValue->SpawnActor<Actor>(FVector::ZeroVector,
                                              FRotator::ZeroRotator)
              : nullptr;
      Spawned ? (ApplyActor(Spawned, Eval.Profile), void()) : void();
    });
  };
}

// --- Subdomain apply entry points ----------------------------------------
// Each is defined in its own RenderingProfile*Thunks.cpp and orchestrated by
// ApplyRuntimeProfile.
void ApplyRuntimeConsoleVariables(
    const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Game::Data::FRenderingSettings &Settings);
void ApplyRuntimeFog(const FRuntimeProfileEval &Eval);
void ApplyRuntimeLighting(const FRuntimeProfileEval &Eval);
void ApplyRuntimeSky(const FRuntimeProfileEval &Eval);
void ApplyRuntimePostProcess(const FRuntimeProfileEval &Eval);
void ApplyRuntimeOutput(const FRuntimeProfileEval &Eval);

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
