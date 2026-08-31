#include "Features/Components/Identity/IdentitySelectors.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Components/Identity/IdentityAdapters.h"
#include "Features/Components/Projection/ProjectionSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace IdentitySelectors {
namespace {

/** User Story: As an entity identity selector, I need to refine a component field to text through Maybe so malformed values do not become sentinels. @fn func::Maybe<FString> SelectTextField(const ecs::FComponentValue &IdentityValue, const char *Field) */
func::Maybe<FString>
SelectTextField(const ecs::FComponentValue &IdentityValue, const char *Field) {
  return func::fmap(
      func::maybe_filter(
          ComponentsAdapters::FindComponentValueField(
              IdentityValue, ComponentsAdapters::ComponentAtom(Field)),
          [](const ecs::FComponentValue &Value) {
            return Value.Kind == ecs::EComponentValueKind::Text;
          }),
      [](const ecs::FComponentValue &Value) { return Value.TextValue; });
}

/** User Story: As an entity identity selector, I need to refine a component field to an integer through Maybe so malformed values do not become sentinels. @fn func::Maybe<int64> SelectIntField(const ecs::FComponentValue &IdentityValue, const char *Field) */
func::Maybe<int64>
SelectIntField(const ecs::FComponentValue &IdentityValue, const char *Field) {
  return func::fmap(
      func::maybe_filter(
          ComponentsAdapters::FindComponentValueField(
              IdentityValue, ComponentsAdapters::ComponentAtom(Field)),
          [](const ecs::FComponentValue &Value) {
            return Value.Kind == ecs::EComponentValueKind::Int;
          }),
      [](const ecs::FComponentValue &Value) { return Value.IntValue; });
}

/** User Story: As an entity identity selector, I need to reconstruct a generation-aware runtime ID only when both fields are present. @fn func::Maybe<ecs::FEntityId> SelectRuntimeId(const ecs::FComponentValue &IdentityValue) */
func::Maybe<ecs::FEntityId>
SelectRuntimeId(const ecs::FComponentValue &IdentityValue) {
  return func::lift2(
      SelectIntField(IdentityValue, FORBOCAI_DEMOUE5_AUTHORED_STRINGVAC9F476EB107),
      SelectIntField(IdentityValue, FORBOCAI_DEMOUE5_AUTHORED_STRINGV5D0C9DC02B8C),
      [](int64 Index, int64 Generation) {
        return ecs::createEntityId(Index, static_cast<int32>(Generation));
      });
}

/** User Story: As an entity identity selector, I need to parse a projected actor GUID through Maybe so malformed actor bindings remain absent. @fn func::Maybe<FGuid> SelectActorGuid(const ecs::FComponentValue &IdentityValue) */
func::Maybe<FGuid>
SelectActorGuid(const ecs::FComponentValue &IdentityValue) {
  return func::mbind(
      SelectTextField(IdentityValue, FORBOCAI_DEMOUE5_AUTHORED_STRINGV7DAD4866CA75),
      [](const FString &Text) {
        FGuid Guid;
        return func::from_nullable_value(Guid, FGuid::Parse(Text, Guid));
      });
}

} // namespace

/** User Story: As an ECS identity consumer, I need to select a projected identity through a stable signature so logical and optional bindings remain inspectable. @fn func::Maybe<FEntityIdentity> SelectEntityIdentity(const ecs::FWorld &World, const ecs::EntityKey &Entity) */
func::Maybe<FEntityIdentity>
SelectEntityIdentity(const ecs::FWorld &World,
                     const ecs::EntityKey &Entity) {
  return func::mbind(
      ecs::getComponent(
          {World, Entity,
           IdentityAdapters::EntityIdentityComponentType()}),
      [](const ecs::FComponentValue &IdentityValue) {
        return func::fmap(
            SelectTextField(IdentityValue, FORBOCAI_DEMOUE5_AUTHORED_STRINGVE80F422E3CD0),
            [&IdentityValue](const FString &StableId) {
              return FEntityIdentity{
                  StableId, SelectRuntimeId(IdentityValue),
                  SelectTextField(IdentityValue, FORBOCAI_DEMOUE5_AUTHORED_STRINGV06108151A0DE),
                  SelectActorGuid(IdentityValue)};
            });
      });
}

/** User Story: As an ECS identity consumer, I need to select a stable logical ID through a stable signature so actor and API code do not inspect raw component maps. @fn func::Maybe<ecs::EntityKey> SelectStableId(const ecs::FWorld &World, const ecs::EntityKey &Entity) */
func::Maybe<ecs::EntityKey>
SelectStableId(const ecs::FWorld &World, const ecs::EntityKey &Entity) {
  return func::fmap(
      SelectEntityIdentity(World, Entity),
      [](const FEntityIdentity &Identity) { return Identity.StableId; });
}

/** User Story: As an ECS identity consumer, I need to select an optional external ID through a stable signature so SDK and API bindings remain explicit. @fn func::Maybe<FString> SelectExternalId(const ecs::FWorld &World, const ecs::EntityKey &Entity) */
func::Maybe<FString>
SelectExternalId(const ecs::FWorld &World, const ecs::EntityKey &Entity) {
  return func::mbind(
      SelectEntityIdentity(World, Entity),
      [](const FEntityIdentity &Identity) { return Identity.ExternalId; });
}

} // namespace IdentitySelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
