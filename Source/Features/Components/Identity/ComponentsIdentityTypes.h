#pragma once

#include "Core/ecs.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FEntityIdentity {
  ecs::EntityKey StableId;
  func::Maybe<ecs::FEntityId> RuntimeId;
  func::Maybe<FString> ExternalId;
  func::Maybe<FGuid> ActorGuid;
};

struct FProjectEntityIdentityPayload {
  ecs::FWorld World;
  FEntityIdentity Identity;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
