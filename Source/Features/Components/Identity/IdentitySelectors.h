#pragma once

#include "Features/Components/Identity/ComponentsIdentityTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace IdentitySelectors {

/** User Story: As an ECS identity consumer, I need to select a projected identity through a stable signature so logical and optional bindings remain inspectable. @fn func::Maybe<FEntityIdentity> SelectEntityIdentity(const ecs::FWorld &World, const ecs::EntityKey &Entity) */
func::Maybe<FEntityIdentity>
SelectEntityIdentity(const ecs::FWorld &World,
                     const ecs::EntityKey &Entity);

/** User Story: As an ECS identity consumer, I need to select a stable logical ID through a stable signature so actor and API code do not inspect raw component maps. @fn func::Maybe<ecs::EntityKey> SelectStableId(const ecs::FWorld &World, const ecs::EntityKey &Entity) */
func::Maybe<ecs::EntityKey>
SelectStableId(const ecs::FWorld &World, const ecs::EntityKey &Entity);

/** User Story: As an ECS identity consumer, I need to select an optional external ID through a stable signature so SDK and API bindings remain explicit. @fn func::Maybe<FString> SelectExternalId(const ecs::FWorld &World, const ecs::EntityKey &Entity) */
func::Maybe<FString>
SelectExternalId(const ecs::FWorld &World, const ecs::EntityKey &Entity);

} // namespace IdentitySelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
