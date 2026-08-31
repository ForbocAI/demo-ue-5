#include "Features/Components/Identity/IdentityAdapters.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Components/Projection/ProjectionSelectors.h"
#include "Features/Components/Value/ComponentsValueAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace IdentityAdapters {

/** User Story: As an entity identity consumer, I need a stable logical identity with absent optional bindings so projected entities never invent external handles. @fn FEntityIdentity CreateStableIdentity(const ecs::EntityKey &StableId) */
FEntityIdentity CreateStableIdentity(const ecs::EntityKey &StableId) {
  return {StableId, func::nothing<ecs::FEntityId>(),
          func::nothing<FString>(), func::nothing<FGuid>()};
}

/** User Story: As an entity identity consumer, I need to bind an allocator identity through a pure value transform so generation-aware handles remain explicit. @fn FEntityIdentity WithRuntimeId(FEntityIdentity Identity, const ecs::FEntityId &RuntimeId) */
FEntityIdentity WithRuntimeId(FEntityIdentity Identity,
                              const ecs::FEntityId &RuntimeId) {
  Identity.RuntimeId = func::just(RuntimeId);
  return Identity;
}

/** User Story: As an entity identity consumer, I need to bind an SDK or API identity through a pure value transform so external references remain optional and inspectable. @fn FEntityIdentity WithExternalId(FEntityIdentity Identity, const FString &ExternalId) */
FEntityIdentity WithExternalId(FEntityIdentity Identity,
                               const FString &ExternalId) {
  Identity.ExternalId = func::just(ExternalId);
  return Identity;
}

/** User Story: As an entity identity consumer, I need to bind an Unreal actor GUID through a pure value transform so actor synchronization does not replace logical identity. @fn FEntityIdentity WithActorGuid(FEntityIdentity Identity, const FGuid &ActorGuid) */
FEntityIdentity WithActorGuid(FEntityIdentity Identity,
                              const FGuid &ActorGuid) {
  Identity.ActorGuid = func::just(ActorGuid);
  return Identity;
}

namespace {

template <typename Value, typename Project>
ecs::FComponentValue OptionalComponentValue(
    const func::Maybe<Value> &OptionalValue, Project ProjectValue) {
  return func::match(
      OptionalValue,
      [ProjectValue](const Value &ValueToProject) {
        return ProjectValue(ValueToProject);
      },
      []() { return ecs::createNoneComponentValue(); });
}

/** User Story: As an entity identity adapter, I need the optional runtime index represented without a sentinel so missing allocator bindings stay explicit. @fn ecs::FComponentValue RuntimeIndexValue(const FEntityIdentity &Identity) */
ecs::FComponentValue RuntimeIndexValue(const FEntityIdentity &Identity) {
  return OptionalComponentValue(
      Identity.RuntimeId, [](const ecs::FEntityId &RuntimeId) {
        return ecs::createIntComponentValue(RuntimeId.Index);
      });
}

/** User Story: As an entity identity adapter, I need the optional generation represented without a sentinel so stale-handle semantics stay explicit. @fn ecs::FComponentValue RuntimeGenerationValue(const FEntityIdentity &Identity) */
ecs::FComponentValue RuntimeGenerationValue(const FEntityIdentity &Identity) {
  return OptionalComponentValue(
      Identity.RuntimeId, [](const ecs::FEntityId &RuntimeId) {
        return ecs::createIntComponentValue(RuntimeId.Generation);
      });
}

/** User Story: As an entity identity adapter, I need the optional external ID represented without an empty-string sentinel so API bindings stay explicit. @fn ecs::FComponentValue ExternalIdValue(const FEntityIdentity &Identity) */
ecs::FComponentValue ExternalIdValue(const FEntityIdentity &Identity) {
  return OptionalComponentValue(
      Identity.ExternalId, [](const FString &ExternalId) {
        return ecs::createTextComponentValue(ExternalId);
      });
}

/** User Story: As an entity identity adapter, I need the optional actor GUID represented without an invalid-GUID sentinel so actor bindings stay explicit. @fn ecs::FComponentValue ActorGuidValue(const FEntityIdentity &Identity) */
ecs::FComponentValue ActorGuidValue(const FEntityIdentity &Identity) {
  return OptionalComponentValue(
      Identity.ActorGuid, [](const FGuid &ActorGuid) {
        return ecs::createTextComponentValue(
            ActorGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
      });
}

} // namespace

/** User Story: As an ECS projection consumer, I need the identity domain path through one public adapter so projection and queries share one definition. @fn TArray<FString> EntityIdentityDomain() */
TArray<FString> EntityIdentityDomain() {
  return ComponentsAdapters::ComponentDomain({"Components", "Identity"});
}

/** User Story: As an ECS projection consumer, I need the identity domain key through one public adapter so projection and queries share one definition. @fn ecs::DomainPathKey EntityIdentityDomainKey() */
ecs::DomainPathKey EntityIdentityDomainKey() {
  return ecs::createDomainPathKey(
      ecs::createDomainPath(EntityIdentityDomain()));
}

/** User Story: As an ECS projection consumer, I need the identity component type through one public adapter so writes and selectors address one table. @fn ecs::ComponentType EntityIdentityComponentType() */
ecs::ComponentType EntityIdentityComponentType() {
  return ComponentsAdapters::ComponentPath(FORBOCAI_DEMOUE5_AUTHORED_STRINGVBDD4395E9CFB,
                                           FORBOCAI_DEMOUE5_AUTHORED_STRINGV09041A212814);
}

/** User Story: As an ECS projection consumer, I need identity represented as a serializable component value so root-world inspection remains generic. @fn ecs::FComponentValue EntityIdentityComponentValue(const FEntityIdentity &Identity) */
ecs::FComponentValue
EntityIdentityComponentValue(const FEntityIdentity &Identity) {
  return ComponentsAdapters::ComponentValueMap(
      {{"StableId", Identity.StableId},
       {"RuntimeIndex", RuntimeIndexValue(Identity)},
       {"Generation", RuntimeGenerationValue(Identity)},
       {"ExternalId", ExternalIdValue(Identity)},
       {"ActorGuid", ActorGuidValue(Identity)}});
}

/** User Story: As an ECS projection consumer, I need an identity component binding so the shared entity catalog can compose identity with domain components. @fn ComponentsAdapters::FComponentProjectionBinding EntityIdentityComponentBinding(const FEntityIdentity &Identity) */
ComponentsAdapters::FComponentProjectionBinding
EntityIdentityComponentBinding(const FEntityIdentity &Identity) {
  return ComponentsAdapters::ComponentBinding(
      EntityIdentityComponentType(), EntityIdentityComponentValue(Identity));
}

/** User Story: As an ECS identity consumer, I need to project enriched identity through one world transform so external and actor bindings stay in the root ECS world. @fn ecs::FWorld ProjectEntityIdentity(const FProjectEntityIdentityPayload &Payload) */
ecs::FWorld
ProjectEntityIdentity(const FProjectEntityIdentityPayload &Payload) {
  return (func::pipe(Payload.World) |
          [&Payload](ecs::FWorld World) {
            return ecs::setEntityDomain(
                {World, Payload.Identity.StableId, EntityIdentityDomainKey()});
          } |
          [&Payload](ecs::FWorld World) {
            return ecs::setComponent(
                {World, Payload.Identity.StableId,
                 EntityIdentityComponentType(),
                 EntityIdentityComponentValue(Payload.Identity)});
          })
      .val;
}

} // namespace IdentityAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
