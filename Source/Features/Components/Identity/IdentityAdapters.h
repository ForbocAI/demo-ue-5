#pragma once

#include "Features/Components/Identity/ComponentsIdentityTypes.h"
#include "Features/Components/Projection/ComponentsProjectionAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace IdentityAdapters {

/** User Story: As an entity identity consumer, I need a stable logical identity with absent optional bindings so projected entities never invent external handles. @fn FEntityIdentity CreateStableIdentity(const ecs::EntityKey &StableId) */
FEntityIdentity CreateStableIdentity(const ecs::EntityKey &StableId);

/** User Story: As an entity identity consumer, I need to bind an allocator identity through a pure value transform so generation-aware handles remain explicit. @fn FEntityIdentity WithRuntimeId(FEntityIdentity Identity, const ecs::FEntityId &RuntimeId) */
FEntityIdentity WithRuntimeId(FEntityIdentity Identity,
                              const ecs::FEntityId &RuntimeId);

/** User Story: As an entity identity consumer, I need to bind an SDK or API identity through a pure value transform so external references remain optional and inspectable. @fn FEntityIdentity WithExternalId(FEntityIdentity Identity, const FString &ExternalId) */
FEntityIdentity WithExternalId(FEntityIdentity Identity,
                               const FString &ExternalId);

/** User Story: As an entity identity consumer, I need to bind an Unreal actor GUID through a pure value transform so actor synchronization does not replace logical identity. @fn FEntityIdentity WithActorGuid(FEntityIdentity Identity, const FGuid &ActorGuid) */
FEntityIdentity WithActorGuid(FEntityIdentity Identity,
                              const FGuid &ActorGuid);

/** User Story: As an ECS projection consumer, I need the identity domain path through one public adapter so projection and queries share one definition. @fn TArray<FString> EntityIdentityDomain() */
TArray<FString> EntityIdentityDomain();

/** User Story: As an ECS projection consumer, I need the identity domain key through one public adapter so projection and queries share one definition. @fn ecs::DomainPathKey EntityIdentityDomainKey() */
ecs::DomainPathKey EntityIdentityDomainKey();

/** User Story: As an ECS projection consumer, I need the identity component type through one public adapter so writes and selectors address one table. @fn ecs::ComponentType EntityIdentityComponentType() */
ecs::ComponentType EntityIdentityComponentType();

/** User Story: As an ECS projection consumer, I need identity represented as a serializable component value so root-world inspection remains generic. @fn ecs::FComponentValue EntityIdentityComponentValue(const FEntityIdentity &Identity) */
ecs::FComponentValue
EntityIdentityComponentValue(const FEntityIdentity &Identity);

/** User Story: As an ECS projection consumer, I need an identity component binding so the shared entity catalog can compose identity with domain components. @fn ComponentsAdapters::FComponentProjectionBinding EntityIdentityComponentBinding(const FEntityIdentity &Identity) */
ComponentsAdapters::FComponentProjectionBinding
EntityIdentityComponentBinding(const FEntityIdentity &Identity);

/** User Story: As an ECS identity consumer, I need to project enriched identity through one world transform so external and actor bindings stay in the root ECS world. @fn ecs::FWorld ProjectEntityIdentity(const FProjectEntityIdentityPayload &Payload) */
ecs::FWorld
ProjectEntityIdentity(const FProjectEntityIdentityPayload &Payload);

} // namespace IdentityAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
